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
#include "ipc/cloud_sync_service.h"

#include <memory>

#include "system_ability_definition.h"

#include "dfs_error.h"
#include "dfsu_access_token_helper.h"
#include "ipc/cloud_sync_callback_manager.h"
#include "meta_file.h"
#include "sync_rule/battery_status.h"
#include "sync_rule/net_conn_callback_observer.h"
#include "sync_rule/network_status.h"
#include "utils_log.h"
#include "directory_ex.h"
#include "sdk_helper.h"

namespace OHOS::FileManagement::CloudSync {
using namespace std;
using namespace OHOS;

REGISTER_SYSTEM_ABILITY_BY_ID(CloudSyncService, FILEMANAGEMENT_CLOUD_SYNC_SERVICE_SA_ID, false);

CloudSyncService::CloudSyncService(int32_t saID, bool runOnCreate) : SystemAbility(saID, runOnCreate)
{
    dataSyncManager_ = make_shared<DataSyncManager>();
    batteryStatusListener_ = make_shared<BatteryStatusListener>(dataSyncManager_);
}

void CloudSyncService::PublishSA()
{
    LOGI("Begin to init");
    if (!SystemAbility::Publish(this)) {
        throw runtime_error(" Failed to publish the daemon");
    }
    LOGI("Init finished successfully");
}

void CloudSyncService::Init()
{
    NetworkStatus::InitNetwork();
    /* Get Init Charging status */
    BatteryStatus::GetInitChargingStatus();
}

std::string CloudSyncService::GetHmdfsPath(const std::string &uri, int32_t userId)
{
    const std::string HMDFS_DIR = "/mnt/hmdfs/";
    const std::string DATA_DIR = "/account/device_view/local/data/";
    const std::string FILE_DIR = "data/storage/el2/distributedfiles/";
    const std::string URI_PREFIX = "://";
    if (uri.empty()) {
        return "";
    }

    std::string bundleName;
    size_t uriPrefixPos = uri.find(URI_PREFIX);
    if (uriPrefixPos == std::string::npos) {
        return "";
    }
    uriPrefixPos += URI_PREFIX.length();
    size_t bundleNameEndPos = uri.find('/', uriPrefixPos);
    if (bundleNameEndPos == std::string::npos) {
        return "";
    }
    bundleName = uri.substr(uriPrefixPos, bundleNameEndPos - uriPrefixPos);

    std::string relativePath;
    size_t fileDirPos = uri.find(FILE_DIR);
    if (fileDirPos == std::string::npos) {
        return "";
    }
    fileDirPos += FILE_DIR.length();
    relativePath = uri.substr(fileDirPos);

    std::string outputPath = HMDFS_DIR + std::to_string(userId) + DATA_DIR + bundleName + "/" + relativePath;
    std::string dir = outputPath.substr(0, outputPath.find_last_of('/'));

    ForceCreateDirectory(dir);
    return outputPath;
}

void CloudSyncService::OnStart()
{
    Init();
    LOGI("Begin to start service");
    try {
        PublishSA();
        AddSystemAbilityListener(COMMON_EVENT_SERVICE_ID);
    } catch (const exception &e) {
        LOGE("%{public}s", e.what());
    }
    LOGI("Start service successfully");
}

void CloudSyncService::OnStop()
{
    LOGI("Stop finished successfully");
}

void CloudSyncService::OnAddSystemAbility(int32_t systemAbilityId, const std::string &deviceId)
{
    LOGI("OnAddSystemAbility systemAbilityId:%{public}d added!", systemAbilityId);
    batteryStatusListener_->Start();
}

int32_t CloudSyncService::UnRegisterCallbackInner()
{
    string bundleName;
    if (DfsuAccessTokenHelper::GetCallerBundleName(bundleName)) {
        return E_INVAL_ARG;
    }

    CloudSyncCallbackManager::GetInstance().RemoveCallback(bundleName);
    return E_OK;
}

int32_t CloudSyncService::RegisterCallbackInner(const sptr<IRemoteObject> &remoteObject)
{
    if (remoteObject == nullptr) {
        LOGE("remoteObject is nullptr");
        return E_INVAL_ARG;
    }

    string bundleName;
    if (DfsuAccessTokenHelper::GetCallerBundleName(bundleName)) {
        return E_INVAL_ARG;
    }

    auto callback = iface_cast<ICloudSyncCallback>(remoteObject);
    auto callerUserId = DfsuAccessTokenHelper::GetUserId();
    CloudSyncCallbackManager::GetInstance().AddCallback(bundleName, callerUserId, callback);
    return E_OK;
}

int32_t CloudSyncService::StartSyncInner(bool forceFlag)
{
    string bundleName;
    if (DfsuAccessTokenHelper::GetCallerBundleName(bundleName)) {
        return E_INVAL_ARG;
    }
    auto callerUserId = DfsuAccessTokenHelper::GetUserId();
    return dataSyncManager_->TriggerStartSync(bundleName, callerUserId, forceFlag, SyncTriggerType::APP_TRIGGER);
}

int32_t CloudSyncService::StopSyncInner()
{
    string bundleName;
    if (DfsuAccessTokenHelper::GetCallerBundleName(bundleName)) {
        return E_INVAL_ARG;
    }
    auto callerUserId = DfsuAccessTokenHelper::GetUserId();
    return dataSyncManager_->TriggerStopSync(bundleName, callerUserId, SyncTriggerType::APP_TRIGGER);
}

int32_t CloudSyncService::ChangeAppSwitch(const std::string &accoutId, const std::string &bundleName, bool status)
{
    auto callerUserId = DfsuAccessTokenHelper::GetUserId();
    if (status) {
        return dataSyncManager_->TriggerStartSync(bundleName, callerUserId, false, SyncTriggerType::CLOUD_TRIGGER);
    }
    return dataSyncManager_->TriggerStopSync(bundleName, callerUserId, SyncTriggerType::CLOUD_TRIGGER);
}

int32_t CloudSyncService::NotifyDataChange(const std::string &accoutId, const std::string &bundleName)
{
    auto callerUserId = DfsuAccessTokenHelper::GetUserId();
    return dataSyncManager_->TriggerStartSync(bundleName, callerUserId, false, SyncTriggerType::CLOUD_TRIGGER);
}

int32_t CloudSyncService::DisableCloud(const std::string &accoutId)
{
    return E_OK;
}

int32_t CloudSyncService::EnableCloud(const std::string &accoutId, const SwitchDataObj &switchData)
{
    return E_OK;
}

int32_t CloudSyncService::Clean(const std::string &accountId, const CleanOptions &cleanOptions)
{
    LOGD("Clean accountId is: %{public}s", accountId.c_str());
    for (auto &iter : cleanOptions.appActionsData) {
        LOGD("Clean key is: %s, value is: %d", iter.first.c_str(), iter.second);
    }

    MetaFileMgr::GetInstance().ClearAll();
    auto callerUserId = DfsuAccessTokenHelper::GetUserId();
    LOGD("Clean callerUserId is: %{public}d", callerUserId);
    std::string appBundleName;
    int32_t action = 0;
    for (auto iter = cleanOptions.appActionsData.begin(); iter != cleanOptions.appActionsData.end(); ++iter) {
        appBundleName = iter->first;
        action = iter->second;
        dataSyncManager_->CleanCloudFile(callerUserId, appBundleName, action);
    }

    return E_OK;
}

constexpr int TEST_MAIN_USR_ID = 100;
int32_t CloudSyncService::StartDownloadFile(const std::string &path)
{
    auto callerUserId = DfsuAccessTokenHelper::GetUserId();
    string bundleName;
    LOGI("start StartDownloadFile");
    if (DfsuAccessTokenHelper::GetCallerBundleName(bundleName)) {
        return E_INVAL_ARG;
    }
    if (callerUserId == 0) {
        callerUserId = TEST_MAIN_USR_ID; // for root user change id to main user for test
    }
    return dataSyncManager_->StartDownloadFile(bundleName, callerUserId, path);
}

int32_t CloudSyncService::StopDownloadFile(const std::string &path)
{
    auto callerUserId = DfsuAccessTokenHelper::GetUserId();
    string bundleName;
    LOGI("start StopDownloadFile");
    if (DfsuAccessTokenHelper::GetCallerBundleName(bundleName)) {
        return E_INVAL_ARG;
    }
    if (callerUserId == 0) {
        callerUserId = TEST_MAIN_USR_ID; // for root user change id to main user for test
    }
    return dataSyncManager_->StopDownloadFile(bundleName, callerUserId, path);
}

int32_t CloudSyncService::RegisterDownloadFileCallback(const sptr<IRemoteObject> &downloadCallback)
{
    auto callerUserId = DfsuAccessTokenHelper::GetUserId();
    auto downloadCb = iface_cast<ICloudDownloadCallback>(downloadCallback);
    string bundleName;
    LOGI("start RegisterDownloadFileCallback");
    if (DfsuAccessTokenHelper::GetCallerBundleName(bundleName)) {
        return E_INVAL_ARG;
    }
    if (callerUserId == 0) {
        callerUserId = TEST_MAIN_USR_ID; // for root user change id to main user for test
    }
    return dataSyncManager_->RegisterDownloadFileCallback(bundleName, callerUserId, downloadCb);
}

int32_t CloudSyncService::UnregisterDownloadFileCallback()
{
    auto callerUserId = DfsuAccessTokenHelper::GetUserId();
    string bundleName;
    LOGI("start UnregisterDownloadFileCallback");
    if (DfsuAccessTokenHelper::GetCallerBundleName(bundleName)) {
        return E_INVAL_ARG;
    }
    if (callerUserId == 0) {
        callerUserId = TEST_MAIN_USR_ID; // for root user change id to main user for test
    }
    return dataSyncManager_->UnregisterDownloadFileCallback(bundleName, callerUserId);
}

int32_t CloudSyncService::UploadAsset(const int32_t userId, const std::string &request, std::string &result)
{
    auto driveKit = DriveKit::DriveKitNative::GetInstance(userId);
    if (driveKit == nullptr) {
        LOGE("uploadAsset get drive kit instance err");
        return E_INVAL_ARG;
    }
    return driveKit->OnUploadAsset(request, result);
}

int32_t CloudSyncService::DownloadFile(const int32_t userId, const std::string &bundleName, AssetInfoObj &assetInfoObj)
{
    auto sdkHelper = std::make_shared<SdkHelper>();
    auto ret = sdkHelper->Init(userId, bundleName);
    if (ret != E_OK) {
        LOGE("get sdk helper err %{public}d", ret);
        return ret;
    }

    DriveKit::DKAsset asset;
    asset.assetName = assetInfoObj.assetName;

    asset.uri = GetHmdfsPath(assetInfoObj.uri, userId);
    if (asset.uri.empty()) {
        LOGE("fail to get download path from %{public}s", assetInfoObj.uri.c_str());
        return E_INVAL_ARG;
    }

    DriveKit::DKDownloadAsset assetsToDownload{
        assetInfoObj.recordType, assetInfoObj.recordId, assetInfoObj.fieldKey, asset, {}};
    return sdkHelper->DownloadAssets(assetsToDownload);
}
} // namespace OHOS::FileManagement::CloudSync
