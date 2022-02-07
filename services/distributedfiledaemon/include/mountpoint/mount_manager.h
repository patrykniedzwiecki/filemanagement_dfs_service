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

#ifndef MOUNT_MANAGER_H
#define MOUNT_MANAGER_H

#include <memory>
#include <mutex>
#include <vector>

#include "mount_point.h"
#include "utils_singleton.h"

namespace OHOS {
namespace Storage {
namespace DistributedFile {
class MountManager final : public Utils::Singleton<MountManager> {
public:
    void Mount(std::unique_ptr<MountPoint> mp);
    void Umount(std::weak_ptr<MountPoint> wmp);
    void Umount(const std::string &groupId);
    DECLARE_SINGLETON(MountManager);

private:
    void StartInstance() override {}
    void StopInstance() override {}
};
} // namespace DistributedFile
} // namespace Storage
} // namespace OHOS
#endif // MOUNT_MANAGER_H