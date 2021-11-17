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

#include "network/session_pool.h"

namespace OHOS {
namespace Storage {
namespace DistributedFile {
using namespace std;

void SessionPool::HoldSession(shared_ptr<BaseSession> session)
{
    lock_guard lock(sessionPoolLock_);
    talker_.SinkSessionTokernel(session);
    AddSessionToPool(session);
    RefreshSessionPoolBasedOnKernel();
}

void SessionPool::RefreshSessionPoolBasedOnKernel()
{
    lock_guard lock(sessionPoolLock_);
    auto kernelSessions = talker_.GetKernelSesions();
    for (auto iter = usrSpaceSessionPool_.begin(); iter != usrSpaceSessionPool_.end();) {
        if (kernelSessions.count((*iter)->GetHandle() == 0)) {
            // (*iter)->Release();
            // iter = usrSpaceSessionPool_.erase(iter);  // ! 待GetKernelSessions实现后放开
        } else {
            ++iter;
        }
    }
}

void SessionPool::AddSessionToPool(shared_ptr<BaseSession> session)
{
    usrSpaceSessionPool_.push_back(session);
}
} // namespace DistributedFile
} // namespace Storage
} // namespace OHOS