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
#include "sync_rule/battery_status.h"
#include "utils_log.h"

namespace OHOS::FileManagement::CloudSync {
constexpr int32_t PAUSE_CAPACITY_LIMIT = 15;
constexpr int32_t STOP_CAPACITY_LIMIT = 10;

bool BatteryStatus::IsCharingStatus()
{
    auto &batterySrvClient = PowerMgr::BatterySrvClient::GetInstance();
    auto chargingStatus = batterySrvClient.GetChargingStatus();
    return chargingStatus == PowerMgr::BatteryChargeState::CHARGE_STATE_ENABLE;
}

int32_t BatteryStatus::GetCapacity()
{
    auto &batterySrvClient = PowerMgr::BatterySrvClient::GetInstance();
    return batterySrvClient.GetCapacity();
}

bool BatteryStatus::IsAllowUpload(bool forceFlag)
{
    if (IsCharingStatus()) {
        return true;
    }

    auto capacity = GetCapacity();
    if (capacity < STOP_CAPACITY_LIMIT) {
        LOGE("power saving, stop upload");
        level_ = BatteryStatus::LEVEL_TOO_LOW;
        return false;
    } else if (capacity < PAUSE_CAPACITY_LIMIT) {
        if (forceFlag) {
            return true;
        } else {
            LOGE("power saving, pause upload");
            level_ = BatteryStatus::LEVEL_LOW;
            return false;
        }
    } else {
        level_ = BatteryStatus::LEVEL_NORMAL;
        return true;
    }
}

bool BatteryStatus::IsBatteryCapcityOkay()
{
    if (IsCharingStatus()) {
        return true;
    }

    auto capacity = GetCapacity();
    if (capacity < STOP_CAPACITY_LIMIT) {
        LOGE("battery capacity too low");
        level_ = BatteryStatus::LEVEL_TOO_LOW;
        return false;
    }
    level_ = BatteryStatus::LEVEL_NORMAL;
    return true;
}

BatteryStatus::CapacityLevel BatteryStatus::GetCapacityLevel()
{
    return level_;
}
} // namespace OHOS::FileManagement::CloudSync