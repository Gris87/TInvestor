#include <gtest/gtest.h>

#include "src/config/decisions/decisionmakerconfig.h"
#include "src/config/decisions/buy/ibuydecision1config_mock.h"
#include "src/config/decisions/buy/ibuydecision2config_mock.h"
#include "src/config/decisions/buy/ibuydecision3config_mock.h"
#include "src/config/decisions/sell/iselldecision1config_mock.h"
#include "src/config/decisions/sell/iselldecision2config_mock.h"
#include "src/config/decisions/sell/iselldecision3config_mock.h"
#include "src/config/isettingseditor_mock.h"



using ::testing::StrictMock;
using ::testing::Return;



TEST(Test_DecisionMakerConfig, Test_constructor_and_destructor)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );
}

TEST(Test_DecisionMakerConfig, Test_assign)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );

    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock2;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock2;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock2;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock2;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock2;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock2;

    DecisionMakerConfig config2(
        &buyDecision1ConfigMock2,
        &buyDecision2ConfigMock2,
        &buyDecision3ConfigMock2,
        &sellDecision1ConfigMock2,
        &sellDecision2ConfigMock2,
        &sellDecision3ConfigMock2
    );

    EXPECT_CALL(buyDecision1ConfigMock, assign(&buyDecision1ConfigMock2));
    EXPECT_CALL(buyDecision2ConfigMock, assign(&buyDecision2ConfigMock2));
    EXPECT_CALL(buyDecision3ConfigMock, assign(&buyDecision3ConfigMock2));
    EXPECT_CALL(sellDecision1ConfigMock, assign(&sellDecision1ConfigMock2));
    EXPECT_CALL(sellDecision2ConfigMock, assign(&sellDecision2ConfigMock2));
    EXPECT_CALL(sellDecision3ConfigMock, assign(&sellDecision3ConfigMock2));

    config.assign(&config2);
}

TEST(Test_DecisionMakerConfig, Test_makeDefault)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );

    EXPECT_CALL(buyDecision1ConfigMock, makeDefault());
    EXPECT_CALL(buyDecision2ConfigMock, makeDefault());
    EXPECT_CALL(buyDecision3ConfigMock, makeDefault());
    EXPECT_CALL(sellDecision1ConfigMock, makeDefault());
    EXPECT_CALL(sellDecision2ConfigMock, makeDefault());
    EXPECT_CALL(sellDecision3ConfigMock, makeDefault());

    config.makeDefault();
}

TEST(Test_DecisionMakerConfig, Test_save)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );

    StrictMock<SettingsEditorMock> settingsEditorMock;

    EXPECT_CALL(buyDecision1ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision1Config")));
    EXPECT_CALL(buyDecision2ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision2Config")));
    EXPECT_CALL(buyDecision3ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision3Config")));
    EXPECT_CALL(sellDecision1ConfigMock, save(&settingsEditorMock, QString("BLAH/SellDecision1Config")));
    EXPECT_CALL(sellDecision2ConfigMock, save(&settingsEditorMock, QString("BLAH/SellDecision2Config")));
    EXPECT_CALL(sellDecision3ConfigMock, save(&settingsEditorMock, QString("BLAH/SellDecision3Config")));

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_DecisionMakerConfig, Test_load)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );

    StrictMock<SettingsEditorMock> settingsEditorMock;

    EXPECT_CALL(buyDecision1ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision1Config")));
    EXPECT_CALL(buyDecision2ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision2Config")));
    EXPECT_CALL(buyDecision3ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision3Config")));
    EXPECT_CALL(sellDecision1ConfigMock, load(&settingsEditorMock, QString("BLAH/SellDecision1Config")));
    EXPECT_CALL(sellDecision2ConfigMock, load(&settingsEditorMock, QString("BLAH/SellDecision2Config")));
    EXPECT_CALL(sellDecision3ConfigMock, load(&settingsEditorMock, QString("BLAH/SellDecision3Config")));

    config.load(&settingsEditorMock, "BLAH");
}

TEST(Test_DecisionMakerConfig, Test_getBuyDecision1Config)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );

    ASSERT_EQ(config.getBuyDecision1Config(), &buyDecision1ConfigMock);
}

TEST(Test_DecisionMakerConfig, Test_getBuyDecision2Config)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );

    ASSERT_EQ(config.getBuyDecision2Config(), &buyDecision2ConfigMock);
}

TEST(Test_DecisionMakerConfig, Test_getBuyDecision3Config)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );

    ASSERT_EQ(config.getBuyDecision3Config(), &buyDecision3ConfigMock);
}

TEST(Test_DecisionMakerConfig, Test_getSellDecision1Config)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );

    ASSERT_EQ(config.getSellDecision1Config(), &sellDecision1ConfigMock);
}

TEST(Test_DecisionMakerConfig, Test_getSellDecision2Config)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );

    ASSERT_EQ(config.getSellDecision2Config(), &sellDecision2ConfigMock);
}

TEST(Test_DecisionMakerConfig, Test_getSellDecision3Config)
{
    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    DecisionMakerConfig config(
        &buyDecision1ConfigMock,
        &buyDecision2ConfigMock,
        &buyDecision3ConfigMock,
        &sellDecision1ConfigMock,
        &sellDecision2ConfigMock,
        &sellDecision3ConfigMock
    );

    ASSERT_EQ(config.getSellDecision3Config(), &sellDecision3ConfigMock);
}
