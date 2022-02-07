/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UTILS_ACTOR_H
#define UTILS_ACTOR_H

#include <algorithm>
#include <future>
#include <list>
#include <stdexcept>
#include <thread>
#include <tuple>

#include "utils_cmd.h"
#include "utils_thread_safe_queue.h"

namespace OHOS {
namespace Storage {
namespace DistributedFile {
constexpr int RETRY_SLEEP_TIME = 1500;
/**
 * @brief  The Actor Model
 *
 * @tparam Ctx Context for Commands
 */
template<typename Ctx>
class Actor {
public:
    explicit Actor(Ctx *ctx, int startCmdTryTimes = 1) : ctx_(ctx), retryTimes_(startCmdTryTimes) {}
    virtual ~Actor() {}

    void StartActor()
    {
        // Always insert cmds b4 starting an actor
        StartCtx();
        loop_ = std::thread(&Actor<Ctx>::Main, this);
    }

    void StopActor()
    {
        // Always insert cmds b4 starting an actor
        StopCtx();
        pendingCmds_.Halt();

        loop_.join();
    }

    void Recv(std::unique_ptr<VirtualCmd<Ctx>> pcmd)
    {
        pendingCmds_.Push(std::move(pcmd));
    }

protected:
    ThreadSafeQueue<VirtualCmd<Ctx>> pendingCmds_;

    Ctx *ctx_{nullptr};
    uint32_t retryTimes_{1};
    std::thread loop_;

    std::list<std::future<void>> retryTasks;

private:
    void DelayRetry(std::unique_ptr<VirtualCmd<Ctx>> cmd)
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_SLEEP_TIME));
        pendingCmds_.Push(std::move(cmd));
    }

    void Retry(std::unique_ptr<VirtualCmd<Ctx>> cmd)
    {
        pendingCmds_.PushFront(std::move(cmd));
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_SLEEP_TIME));
    }

    void StartCtx()
    {
        auto startCmd = std::make_unique<Cmd<Ctx>>(&Ctx::Start);
        startCmd->UpdateOption({
            .importance_ = CmdImportance::SUBVITAL,
            .tryTimes_ = retryTimes_,
        });
        pendingCmds_.Push(std::move(startCmd));
    }

    void StopCtx()
    {
        auto cmd = std::make_unique<Cmd<Ctx>>(&Ctx::Stop);
        cmd->UpdateOption({
            .importance_ = CmdImportance::VITAL,
            .tryTimes_ = 1,
        });
        pendingCmds_.Push(std::move(cmd));
    }

    bool IsExistStopTask()
    {
        bool result = false;
        pendingCmds_.ForEach([&result](const std::unique_ptr<VirtualCmd<Ctx>> &item) {
            if (item->option_.importance_ == CmdImportance::VITAL) {
                result = true;
            }
        });
        return result;
    }

    void Main()
    {
        while (true) {
            std::unique_ptr<VirtualCmd<Ctx>> currentCmd;
            try {
                while (true) {
                    currentCmd = pendingCmds_.WaitAndPop();
                    (*currentCmd)(ctx_);
                    currentCmd.release();
                }
            } catch (const Exception &e) {
                if (e.code() == ERR_UTILS_ACTOR_QUEUE_STOP) {
                    break;
                }

                const CmdOptions &op = currentCmd->option_;

                if (IsExistStopTask() && (op.tryTimes_ > 0)) {
                    break; // exist stop Task, stop retry
                }

                if (op.importance_ == CmdImportance::TRIVIAL) {
                    if (op.tryTimes_) {
                        retryTasks.emplace_back(
                            std::async(std::launch::async, &Actor<Ctx>::DelayRetry, this, std::move(currentCmd)));
                        continue;
                    }
                } else {
                    if (op.tryTimes_) {
                        Retry(std::move(currentCmd));
                        continue;
                    }
                    if (op.importance_ == CmdImportance::VITAL) {
                        break;
                    }
                    if (op.importance_ == CmdImportance::NORMAL) {
                        StopCtx();
                        StartCtx();
                        continue;
                    }
                }
            } catch (const std::exception &e) {
                LOGE("Unexpected Low Level exception");
            }
        }
    }
};
} // namespace DistributedFile
} // namespace Storage
} // namespace OHOS
#endif // UTILS_ACTOR_H