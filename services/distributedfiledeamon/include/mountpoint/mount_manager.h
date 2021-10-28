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

#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "mount_point.h"
#include "utils_singleton.h"

namespace OHOS {
namespace DistributedFile {
class MountManager final : public Utils::Singleton<MountManager> {
public:
    void Mount(std::unique_ptr<MountPoint> mp);
    void Umount(std::weak_ptr<MountPoint> wmp);

    DECLARE_SINGLETON(MountManager);

private:
    void Start() override {}
    void Stop() override {}

    std::mutex serializer_;
    std::vector<std::shared_ptr<MountPoint>> mountPoints_;
};
} // namespace DistributedFile
} // namespace OHOS