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

#ifndef OHOS_FILEMGMT_CLOUD_SYNC_COMMON_H
#define OHOS_FILEMGMT_CLOUD_SYNC_COMMON_H

#include <map>

#include "parcel.h"

namespace OHOS::FileManagement::CloudSync {
struct SwitchDataObj : public Parcelable {
    std::map<std::string, bool> switchData;
    bool ReadFromParcel(Parcel &parcel);
    bool Marshalling(Parcel &parcel) const override;
    static SwitchDataObj *Unmarshalling(Parcel &parcel);
};

struct CleanOptions : public Parcelable {
    std::map<std::string, int32_t> appActionsData;
    bool ReadFromParcel(Parcel &parcel);
    bool Marshalling(Parcel &parcel) const override;
    static CleanOptions *Unmarshalling(Parcel &parcel);
};
}
#endif
