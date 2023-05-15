/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "ipc/cloud_process_callback_proxy.h"

#include <sstream>

#include "dfs_error.h"
#include "utils_log.h"

namespace OHOS::FileManagement::CloudSync {
using namespace std;

void CloudProcessCallbackProxy::OnDownloadProcess(int64_t downloadedSize, int64_t totalSize)
{
    LOGI("Start");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        LOGE("Failed to write interface token");
        return;
    }

    if (!data.WriteInt64(downloadedSize)) {
        LOGE("Failed to send the type");
        return;
    }

    if (!data.WriteInt64(totalSize)) {
        LOGE("Failed to send the state");
        return;
    }

    auto remote = Remote();
    if (!remote) {
        LOGE("remote is nullptr");
        return;
    }
    int32_t ret = remote->SendRequest(ICloudProcessCallback::SERVICE_CMD_ON_PROCESS, data, reply, option);
    if (ret != E_OK) {
        stringstream ss;
        ss << "Failed to send out the requeset, errno:" << ret;
        LOGE("%{public}s", ss.str().c_str());
        return;
    }
    LOGI("End");
    return;
}
} // namespace OHOS::FileManagement::CloudSync
