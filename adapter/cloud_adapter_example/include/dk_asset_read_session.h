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

#ifndef DRIVE_KIT_ASSET_READ_SESSION_H
#define DRIVE_KIT_ASSET_READ_SESSION_H

#include "dk_error.h"

namespace DriveKit {
enum class DKSeekFromWhere {
    DK_SEEK_SET,
    DK_SEEK_CUR,
    DK_SEEK_END,
};

class DKAssetReadSession {
public:
    void GetAssetInfo();
    void Seek(int64_t offset, DKSeekFromWhere fromWhere);
    int64_t Read(int64_t size, char *buffer, DKError &error);
    void Close();
};
} // namespace DriveKit
#endif
