/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_FILEMGMT_CLOUD_SYNC_CONSTANTS_H
#define OHOS_FILEMGMT_CLOUD_SYNC_CONSTANTS_H

namespace OHOS::FileManagement::CloudSync {
enum class SyncType : int32_t {
    UPLOAD = 0,
    DOWNLOAD,
    ALL,
};

enum class SyncPromptState : int32_t {
    SYNC_STATE_DEFAULT = 0,
    SYNC_STATE_SYNCING,
    SYNC_STATE_FAILED,
    SYNC_STATE_PAUSED_NO_NETWORK,
    SYNC_STATE_PAUSED_FOR_WIFI,
    SYNC_STATE_PAUSED_FOR_BATTERY,
    SYNC_STATE_BATTERY_TOO_LOW,
    SYNC_STATE_PAUSED_FOR_SPACE_TOO_LOW,
};

} // namespace OHOS::FileManagement::CloudSync

#endif // OHOS_FILEMGMT_CLOUD_SYNC_CONSTANTS_H