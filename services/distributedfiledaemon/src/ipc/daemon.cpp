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

#include "ipc/daemon.h"

#include "mountpoint/mount_manager.h"
#include "system_ability_definition.h"
#include "utils_log.h"

namespace OHOS {
namespace Storage {
namespace DistributedFile {
using namespace std;

REGISTER_SYSTEM_ABILITY_BY_ID(Daemon, STORAGE_DISTRIBUTED_FILE_DAEMON_SA_ID, true);

void Daemon::PublishSA()
{
    LOGI("Begin to init");
    if (!registerToService_) {
        bool ret = SystemAbility::Publish(this);
        if (!ret) {
            throw runtime_error("Failed to publish the daemon");
        }
        registerToService_ = true;
    }
    LOGI("Init finished successfully");
}

void Daemon::StartManagers()
{
    MountManager::GetInstance();
}

void Daemon::OnStart()
{
    LOGI("Begin to start service");
    if (state_ == ServiceRunningState::STATE_RUNNING) {
        LOGD("Daemon has already started");
        return;
    }

    try {
        PublishSA();
        StartManagers();
    } catch (const exception &e) {
        LOGE("%{public}s", e.what());
    }

    state_ = ServiceRunningState::STATE_RUNNING;
    LOGI("Start service successfully");
}

void Daemon::OnStop()
{
    LOGI("Begin to stop");
    state_ = ServiceRunningState::STATE_NOT_START;
    registerToService_ = false;
    LOGI("Stop finished successfully");
}

int32_t Daemon::EchoServerDemo(const string &echoStr)
{
    return 0;
}
} // namespace DistributedFile
} // namespace Storage
} // namespace OHOS