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

#ifndef UTILS_STARTABLE_H
#define UTILS_STARTABLE_H

namespace OHOS {
namespace Storage {
namespace DistributedFile {
class Startable {
public:
    virtual void Start() = 0;
    virtual void Stop() = 0;
};
} // namespace DistributedFile
} // namespace Storage
} // namespace OHOS
#endif // UTILS_STARTABLE_H