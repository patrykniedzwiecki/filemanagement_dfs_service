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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "gallery_data_syncer.h"

#include "dfs_error.h"

namespace OHOS::FileManagement::CloudSync::Test {
using namespace testing;
using namespace testing::ext;
using namespace std;

const int USER_ID = 100;
const std::string BUND_NAME = "com.ohos.photos";
class GalleryDataSyncerTest : public testing::Test {
public:
    enum Action {
        RETAIN_DATA = 0,
        CLEAR_DATA
    };
    enum {
        BEGIN,
        PREPARE,
        DOWNLOADALBUM,
        DOWNLOADFILE,
        UPLOADALBUM,
        UPLOADFILE,
        WAIT,
        END
    };
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
    shared_ptr<GalleryDataSyncer> galleryDataSyncer_;
};
void GalleryDataSyncerTest::SetUpTestCase(void)
{
    GTEST_LOG_(INFO) << "SetUpTestCase";
}

void GalleryDataSyncerTest::TearDownTestCase(void)
{
    GTEST_LOG_(INFO) << "TearDownTestCase";
}

void GalleryDataSyncerTest::SetUp(void)
{
    GTEST_LOG_(INFO) << "SetUp";
    galleryDataSyncer_ = make_shared<GalleryDataSyncer>(BUND_NAME, USER_ID);
}

void GalleryDataSyncerTest::TearDown(void)
{
    GTEST_LOG_(INFO) << "TearDown";
}

/**
 * @tc.name: CleanTest
 * @tc.desc: Verify the Clean function
 * @tc.type: FUNC
 * @tc.require: I6JPKG
 */
HWTEST_F(GalleryDataSyncerTest, CleanTest, TestSize.Level1)
{
    int action = CLEAR_DATA;
    int res = galleryDataSyncer_->Clean(action);
    EXPECT_EQ(res, E_OK);
}
} // namespace OHOS::FileManagement::CloudSync::Test