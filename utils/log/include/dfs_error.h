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

#ifndef OHOS_FILEMGMT_DFS_ERROR_H
#define OHOS_FILEMGMT_DFS_ERROR_H

#include <string_view>
#include <sys/types.h>
#include <unordered_map>

#include "errors.h"

namespace OHOS::FileManagement {
constexpr std::string_view SPACE_NOT_ENOUGH = "30184039";
constexpr std::string_view AT_FAILED = "31000401";
constexpr std::string_view NAME_CONFLICT = "31084932";
constexpr std::string_view INVALID_FILE = "31004002";
constexpr int STORAGE_SERVICE_SYS_CAP_TAG = 13600000;
constexpr int DISTRIBUTEDFILE_SERVICE_SYS_CAP_TAG = 22400000;

enum CloudSyncServiceErrCode : ErrCode {
    E_OK = 0,
    E_SEVICE_DIED,
    E_INVAL_ARG,
    E_BROKEN_IPC,
    E_SA_LOAD_FAILED,
    E_SERVICE_DESCRIPTOR_IS_EMPTY,
    E_PERMISSION_DENIED,
    E_PERMISSION_SYSTEM,
    E_GET_TOKEN_INFO_ERROR,
    E_SYNCER_NUM_OUT_OF_RANGE,
    E_SYNC_FAILED_CLOUD_NOT_READY,
    E_SYNC_FAILED_BATTERY_LOW,
    E_SYNC_FAILED_BATTERY_TOO_LOW,
    E_SYNC_FAILED_NETWORK_NOT_AVAILABLE,
    E_GET_NETWORK_MANAGER_FAILED,

    /* data syncer */
    E_CLOUD_SDK,
    E_RDB,
    E_CONTEXT,
    E_STOP,
    E_PENDING,
    E_SCHEDULE,
    E_ASYNC_RUN,
    E_PATH
};

enum JsErrCode {
    E_PERMISSION = 201,
    E_PERMISSION_SYS = 202,
    E_PARAMS = 401,
    E_IPCSS = STORAGE_SERVICE_SYS_CAP_TAG + 1,
    E_CLOUD_NOT_READY = DISTRIBUTEDFILE_SERVICE_SYS_CAP_TAG + 1,
    E_NETWORK_ERR = DISTRIBUTEDFILE_SERVICE_SYS_CAP_TAG + 2,
    E_BATTERY_WARNING = DISTRIBUTEDFILE_SERVICE_SYS_CAP_TAG + 3,
};

const std::unordered_map<int32_t, int32_t> errCodeTable {
    { E_PERMISSION_DENIED, E_PERMISSION },
    { E_PERMISSION_SYSTEM, E_PERMISSION_SYS },
    { E_INVAL_ARG, E_PARAMS },
    { E_BROKEN_IPC, E_IPCSS },
    { E_SYNC_FAILED_CLOUD_NOT_READY, E_CLOUD_NOT_READY },
    { E_SYNC_FAILED_NETWORK_NOT_AVAILABLE, E_NETWORK_ERR },
    { E_SYNC_FAILED_BATTERY_LOW, E_BATTERY_WARNING },
};

int32_t Convert2JsErrNum(int32_t errNum);
} // namespace OHOS::FileManagement

#endif // OHOS_FILEMGMT_DFS_ERROR_H
