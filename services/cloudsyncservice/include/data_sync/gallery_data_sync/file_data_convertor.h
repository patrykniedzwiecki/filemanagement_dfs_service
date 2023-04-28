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

#ifndef OHOS_CLOUD_SYNC_SERVICE_FILE_DATA_CONVERTOR_H
#define OHOS_CLOUD_SYNC_SERVICE_FILE_DATA_CONVERTOR_H

#include <unordered_map>

#include "data_convertor.h"

namespace OHOS {
namespace FileManagement {
namespace CloudSync {
class FileDataConvertor : public DataConvertor {
public:
    FileDataConvertor(int32_t userId, std::string &bundleName, bool isNew);
    ~FileDataConvertor() = default;

    int32_t Convert(DriveKit::DKRecord &record, NativeRdb::ResultSet &resultSet);

private:
    /* record id */
    int32_t FillRecordId(DriveKit::DKRecord &record, NativeRdb::ResultSet &resultSet);

    /* basic */
    int32_t HandleAlbumId(std::string &key, DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet);
    int32_t HandleFileName(std::string &key, DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet);
    int32_t HandleHashId(std::string &key, DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet);
    int32_t HandleSource(std::string &key, DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet);
    int32_t HandleFileType(std::string &key, DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet);
    int32_t HandleCreatedTime(std::string &key, DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet);
    int32_t HandleFavorite(std::string &key, DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet);
    int32_t HandleDescription(std::string &key, DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet);

    /* properties */
    int32_t HandleProperties(std::string &key, DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet);

    int32_t HandleHeight(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandleRotation(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandleWidth(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandlePosition(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandleDataModified(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandleDetailTime(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandleFileCreateTime(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandleFirstUpdateTime(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandleRelativeBucketId(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandleSourceFileName(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandleSourcePath(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);
    int32_t HandleTimeZone(std::string &key, DriveKit::DKRecordFieldMap &map, NativeRdb::ResultSet &resultSet);

    /* attachments */
    int32_t HandleAttachments(std::string &key, DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet);

    int32_t HandleContent(DriveKit::DKRecordFieldList &list, NativeRdb::ResultSet &resultSet);
    int32_t HandleThumbnail(DriveKit::DKRecordFieldList &list, NativeRdb::ResultSet &resultSet);
    int32_t HandleLcd(DriveKit::DKRecordFieldList &list, NativeRdb::ResultSet &resultSet);

    /* identifier */
    int32_t userId_;
    std::string bundleName_;
    static std::string recordType_;
    bool isNew_;

    /* path */
    static std::string realPrefix_;
    static std::string suffix_;
    static std::string sandboxPrefix_;

    /* map */
    static std::unordered_map<std::string, int32_t (FileDataConvertor::*)(std::string &key,
        DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet)> map_;
    static std::unordered_map<std::string, int32_t (FileDataConvertor::*)(std::string &key,
        DriveKit::DKRecordData &data, NativeRdb::ResultSet &resultSet)> pMap_;
    static std::unordered_map<std::string, int32_t (FileDataConvertor::*)(DriveKit::DKRecordFieldList &list,
        NativeRdb::ResultSet &resultSet)> aMap_;
};
} // namespace CloudSync
} // namespace FileManagement
} // namespace OHOS
#endif // OHOS_CLOUD_SYNC_SERVICE_FILE_DATA_CONVERTOR_H