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

#ifndef I_CLOUD_DAEMON_H
#define I_CLOUD_DAEMON_H

#include "iremote_broker.h"

namespace OHOS {
namespace Storage {
namespace CloudFile {
class ICloudDaemon : public IRemoteBroker {
public:
    enum {
        CLOUD_DAEMON_CMD_ECHO = 0,
    };
    enum {
        CLOUD_DAEMON_SUCCESS = 0,
        CLOUD_DAEMON_DESCRIPTOR_IS_EMPTY,
    };
    virtual int32_t EchoServerDemo(const std::string &echoStr) = 0;

    static inline const std::string SERVICE_NAME { "ohos.storage.distributedfile.clouddaemon" };
    DECLARE_INTERFACE_DESCRIPTOR(u"ohos.storage.distributedfile.clouddaemon")
};
} // namespace CloudFile
} // namespace Storage
} // namespace OHOS
#endif // I_CLOUD_DAEMON_H
