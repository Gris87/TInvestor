#include "src/config/decisions/decisionmakerconfig.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/ibuydecision1config_mock.h"
#include "src/config/decisions/buy/buydecision2config/ibuydecision2config_mock.h"
#include "src/config/decisions/buy/buydecision3config/ibuydecision3config_mock.h"
#include "src/config/decisions/buy/buydecision4config/ibuydecision4config_mock.h"
#include "src/config/decisions/buy/buydecision5config/ibuydecision5config_mock.h"
#include "src/config/decisions/buy/buydecision6config/ibuydecision6config_mock.h"
#include "src/config/decisions/buy/buydecision7config/ibuydecision7config_mock.h"
#include "src/config/decisions/buy/buydecision8config/ibuydecision8config_mock.h"
#include "src/config/decisions/sell/selldecision1config/iselldecision1config_mock.h"
#include "src/config/decisions/sell/selldecision2config/iselldecision2config_mock.h"
#include "src/config/decisions/sell/selldecision3config/iselldecision3config_mock.h"
#include "src/config/decisions/sell/selldecision4config/iselldecision4config_mock.h"
#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



class Test_DecisionMakerConfig : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision1ConfigMock  = new StrictMock<BuyDecision1ConfigMock>();
        buyDecision2ConfigMock  = new StrictMock<BuyDecision2ConfigMock>();
        buyDecision3ConfigMock  = new StrictMock<BuyDecision3ConfigMock>();
        buyDecision4ConfigMock  = new StrictMock<BuyDecision4ConfigMock>();
        buyDecision5ConfigMock  = new StrictMock<BuyDecision5ConfigMock>();
        buyDecision6ConfigMock  = new StrictMock<BuyDecision6ConfigMock>();
        buyDecision7ConfigMock  = new StrictMock<BuyDecision7ConfigMock>();
        buyDecision8ConfigMock  = new StrictMock<BuyDecision8ConfigMock>();
        sellDecision1ConfigMock = new StrictMock<SellDecision1ConfigMock>();
        sellDecision2ConfigMock = new StrictMock<SellDecision2ConfigMock>();
        sellDecision3ConfigMock = new StrictMock<SellDecision3ConfigMock>();
        sellDecision4ConfigMock = new StrictMock<SellDecision4ConfigMock>();

        config = new DecisionMakerConfig(
            buyDecision1ConfigMock,
            buyDecision2ConfigMock,
            buyDecision3ConfigMock,
            buyDecision4ConfigMock,
            buyDecision5ConfigMock,
            buyDecision6ConfigMock,
            buyDecision7ConfigMock,
            buyDecision8ConfigMock,
            sellDecision1ConfigMock,
            sellDecision2ConfigMock,
            sellDecision3ConfigMock,
            sellDecision4ConfigMock
        );
    }

    void TearDown() override
    {
        delete config;
        delete buyDecision1ConfigMock;
        delete buyDecision2ConfigMock;
        delete buyDecision3ConfigMock;
        delete buyDecision4ConfigMock;
        delete buyDecision5ConfigMock;
        delete buyDecision6ConfigMock;
        delete buyDecision7ConfigMock;
        delete buyDecision8ConfigMock;
        delete sellDecision1ConfigMock;
        delete sellDecision2ConfigMock;
        delete sellDecision3ConfigMock;
        delete sellDecision4ConfigMock;
    }

    DecisionMakerConfig*                 config;
    StrictMock<BuyDecision1ConfigMock>*  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>*  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>*  buyDecision3ConfigMock;
    StrictMock<BuyDecision4ConfigMock>*  buyDecision4ConfigMock;
    StrictMock<BuyDecision5ConfigMock>*  buyDecision5ConfigMock;
    StrictMock<BuyDecision6ConfigMock>*  buyDecision6ConfigMock;
    StrictMock<BuyDecision7ConfigMock>*  buyDecision7ConfigMock;
    StrictMock<BuyDecision8ConfigMock>*  buyDecision8ConfigMock;
    StrictMock<SellDecision1ConfigMock>* sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock>* sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock>* sellDecision3ConfigMock;
    StrictMock<SellDecision4ConfigMock>* sellDecision4ConfigMock;
};



TEST_F(Test_DecisionMakerConfig, Test_constructor_and_destructor)
{
}

TEST_F(Test_DecisionMakerConfig, Test_clone_and_deleteRecursively)
{
    const InSequence seq;

    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock2;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock2;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock2;
    StrictMock<BuyDecision4ConfigMock>  buyDecision4ConfigMock2;
    StrictMock<BuyDecision5ConfigMock>  buyDecision5ConfigMock2;
    StrictMock<BuyDecision6ConfigMock>  buyDecision6ConfigMock2;
    StrictMock<BuyDecision7ConfigMock>  buyDecision7ConfigMock2;
    StrictMock<BuyDecision8ConfigMock>  buyDecision8ConfigMock2;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock2;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock2;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock2;
    StrictMock<SellDecision4ConfigMock> sellDecision4ConfigMock2;

    EXPECT_CALL(*buyDecision1ConfigMock, clone()).WillOnce(Return(&buyDecision1ConfigMock2));
    EXPECT_CALL(*buyDecision2ConfigMock, clone()).WillOnce(Return(&buyDecision2ConfigMock2));
    EXPECT_CALL(*buyDecision3ConfigMock, clone()).WillOnce(Return(&buyDecision3ConfigMock2));
    EXPECT_CALL(*buyDecision4ConfigMock, clone()).WillOnce(Return(&buyDecision4ConfigMock2));
    EXPECT_CALL(*buyDecision5ConfigMock, clone()).WillOnce(Return(&buyDecision5ConfigMock2));
    EXPECT_CALL(*buyDecision6ConfigMock, clone()).WillOnce(Return(&buyDecision6ConfigMock2));
    EXPECT_CALL(*buyDecision7ConfigMock, clone()).WillOnce(Return(&buyDecision7ConfigMock2));
    EXPECT_CALL(*buyDecision8ConfigMock, clone()).WillOnce(Return(&buyDecision8ConfigMock2));
    EXPECT_CALL(*sellDecision1ConfigMock, clone()).WillOnce(Return(&sellDecision1ConfigMock2));
    EXPECT_CALL(*sellDecision2ConfigMock, clone()).WillOnce(Return(&sellDecision2ConfigMock2));
    EXPECT_CALL(*sellDecision3ConfigMock, clone()).WillOnce(Return(&sellDecision3ConfigMock2));
    EXPECT_CALL(*sellDecision4ConfigMock, clone()).WillOnce(Return(&sellDecision4ConfigMock2));
    EXPECT_CALL(buyDecision1ConfigMock2, assign(buyDecision1ConfigMock));
    EXPECT_CALL(buyDecision2ConfigMock2, assign(buyDecision2ConfigMock));
    EXPECT_CALL(buyDecision3ConfigMock2, assign(buyDecision3ConfigMock));
    EXPECT_CALL(buyDecision4ConfigMock2, assign(buyDecision4ConfigMock));
    EXPECT_CALL(buyDecision5ConfigMock2, assign(buyDecision5ConfigMock));
    EXPECT_CALL(buyDecision6ConfigMock2, assign(buyDecision6ConfigMock));
    EXPECT_CALL(buyDecision7ConfigMock2, assign(buyDecision7ConfigMock));
    EXPECT_CALL(buyDecision8ConfigMock2, assign(buyDecision8ConfigMock));
    EXPECT_CALL(sellDecision1ConfigMock2, assign(sellDecision1ConfigMock));
    EXPECT_CALL(sellDecision2ConfigMock2, assign(sellDecision2ConfigMock));
    EXPECT_CALL(sellDecision3ConfigMock2, assign(sellDecision3ConfigMock));
    EXPECT_CALL(sellDecision4ConfigMock2, assign(sellDecision4ConfigMock));

    IDecisionMakerConfig* config2 = config->clone();

    EXPECT_CALL(buyDecision1ConfigMock2, deleteRecursively());
    EXPECT_CALL(buyDecision2ConfigMock2, deleteRecursively());
    EXPECT_CALL(buyDecision3ConfigMock2, deleteRecursively());
    EXPECT_CALL(buyDecision4ConfigMock2, deleteRecursively());
    EXPECT_CALL(buyDecision5ConfigMock2, deleteRecursively());
    EXPECT_CALL(buyDecision6ConfigMock2, deleteRecursively());
    EXPECT_CALL(buyDecision7ConfigMock2, deleteRecursively());
    EXPECT_CALL(buyDecision8ConfigMock2, deleteRecursively());
    EXPECT_CALL(sellDecision1ConfigMock2, deleteRecursively());
    EXPECT_CALL(sellDecision2ConfigMock2, deleteRecursively());
    EXPECT_CALL(sellDecision3ConfigMock2, deleteRecursively());
    EXPECT_CALL(sellDecision4ConfigMock2, deleteRecursively());

    config2->deleteRecursively();
}

TEST_F(Test_DecisionMakerConfig, Test_assign)
{
    const InSequence seq;

    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock2;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock2;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock2;
    StrictMock<BuyDecision4ConfigMock>  buyDecision4ConfigMock2;
    StrictMock<BuyDecision5ConfigMock>  buyDecision5ConfigMock2;
    StrictMock<BuyDecision6ConfigMock>  buyDecision6ConfigMock2;
    StrictMock<BuyDecision7ConfigMock>  buyDecision7ConfigMock2;
    StrictMock<BuyDecision8ConfigMock>  buyDecision8ConfigMock2;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock2;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock2;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock2;
    StrictMock<SellDecision4ConfigMock> sellDecision4ConfigMock2;

    DecisionMakerConfig config2(
        &buyDecision1ConfigMock2,
        &buyDecision2ConfigMock2,
        &buyDecision3ConfigMock2,
        &buyDecision4ConfigMock2,
        &buyDecision5ConfigMock2,
        &buyDecision6ConfigMock2,
        &buyDecision7ConfigMock2,
        &buyDecision8ConfigMock2,
        &sellDecision1ConfigMock2,
        &sellDecision2ConfigMock2,
        &sellDecision3ConfigMock2,
        &sellDecision4ConfigMock2
    );

    EXPECT_CALL(*buyDecision1ConfigMock, assign(&buyDecision1ConfigMock2));
    EXPECT_CALL(*buyDecision2ConfigMock, assign(&buyDecision2ConfigMock2));
    EXPECT_CALL(*buyDecision3ConfigMock, assign(&buyDecision3ConfigMock2));
    EXPECT_CALL(*buyDecision4ConfigMock, assign(&buyDecision4ConfigMock2));
    EXPECT_CALL(*buyDecision5ConfigMock, assign(&buyDecision5ConfigMock2));
    EXPECT_CALL(*buyDecision6ConfigMock, assign(&buyDecision6ConfigMock2));
    EXPECT_CALL(*buyDecision7ConfigMock, assign(&buyDecision7ConfigMock2));
    EXPECT_CALL(*buyDecision8ConfigMock, assign(&buyDecision8ConfigMock2));
    EXPECT_CALL(*sellDecision1ConfigMock, assign(&sellDecision1ConfigMock2));
    EXPECT_CALL(*sellDecision2ConfigMock, assign(&sellDecision2ConfigMock2));
    EXPECT_CALL(*sellDecision3ConfigMock, assign(&sellDecision3ConfigMock2));
    EXPECT_CALL(*sellDecision4ConfigMock, assign(&sellDecision4ConfigMock2));

    config->assign(&config2);
}

TEST_F(Test_DecisionMakerConfig, Test_makeDefault)
{
    const InSequence seq;

    EXPECT_CALL(*buyDecision1ConfigMock, makeDefault());
    EXPECT_CALL(*buyDecision2ConfigMock, makeDefault());
    EXPECT_CALL(*buyDecision3ConfigMock, makeDefault());
    EXPECT_CALL(*buyDecision4ConfigMock, makeDefault());
    EXPECT_CALL(*buyDecision5ConfigMock, makeDefault());
    EXPECT_CALL(*buyDecision6ConfigMock, makeDefault());
    EXPECT_CALL(*buyDecision7ConfigMock, makeDefault());
    EXPECT_CALL(*buyDecision8ConfigMock, makeDefault());
    EXPECT_CALL(*sellDecision1ConfigMock, makeDefault());
    EXPECT_CALL(*sellDecision2ConfigMock, makeDefault());
    EXPECT_CALL(*sellDecision3ConfigMock, makeDefault());
    EXPECT_CALL(*sellDecision4ConfigMock, makeDefault());

    config->makeDefault();
}

TEST_F(Test_DecisionMakerConfig, Test_save)
{
    const InSequence seq;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(*buyDecision1ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision1Config")));
    EXPECT_CALL(*buyDecision2ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision2Config")));
    EXPECT_CALL(*buyDecision3ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision3Config")));
    EXPECT_CALL(*buyDecision4ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision4Config")));
    EXPECT_CALL(*buyDecision5ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision5Config")));
    EXPECT_CALL(*buyDecision6ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision6Config")));
    EXPECT_CALL(*buyDecision7ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision7Config")));
    EXPECT_CALL(*buyDecision8ConfigMock, save(&settingsEditorMock,  QString("BLAH/BuyDecision8Config")));
    EXPECT_CALL(*sellDecision1ConfigMock, save(&settingsEditorMock, QString("BLAH/SellDecision1Config")));
    EXPECT_CALL(*sellDecision2ConfigMock, save(&settingsEditorMock, QString("BLAH/SellDecision2Config")));
    EXPECT_CALL(*sellDecision3ConfigMock, save(&settingsEditorMock, QString("BLAH/SellDecision3Config")));
    EXPECT_CALL(*sellDecision4ConfigMock, save(&settingsEditorMock, QString("BLAH/SellDecision4Config")));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_DecisionMakerConfig, Test_load)
{
    const InSequence seq;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(*buyDecision1ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision1Config")));
    EXPECT_CALL(*buyDecision2ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision2Config")));
    EXPECT_CALL(*buyDecision3ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision3Config")));
    EXPECT_CALL(*buyDecision4ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision4Config")));
    EXPECT_CALL(*buyDecision5ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision5Config")));
    EXPECT_CALL(*buyDecision6ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision6Config")));
    EXPECT_CALL(*buyDecision7ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision7Config")));
    EXPECT_CALL(*buyDecision8ConfigMock, load(&settingsEditorMock,  QString("BLAH/BuyDecision8Config")));
    EXPECT_CALL(*sellDecision1ConfigMock, load(&settingsEditorMock, QString("BLAH/SellDecision1Config")));
    EXPECT_CALL(*sellDecision2ConfigMock, load(&settingsEditorMock, QString("BLAH/SellDecision2Config")));
    EXPECT_CALL(*sellDecision3ConfigMock, load(&settingsEditorMock, QString("BLAH/SellDecision3Config")));
    EXPECT_CALL(*sellDecision4ConfigMock, load(&settingsEditorMock, QString("BLAH/SellDecision4Config")));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");
}

TEST_F(Test_DecisionMakerConfig, Test_fromJsonObject)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*buyDecision1ConfigMock,  fromJsonObject(_));
    EXPECT_CALL(*buyDecision2ConfigMock,  fromJsonObject(_));
    EXPECT_CALL(*buyDecision3ConfigMock,  fromJsonObject(_));
    EXPECT_CALL(*buyDecision4ConfigMock,  fromJsonObject(_));
    EXPECT_CALL(*buyDecision5ConfigMock,  fromJsonObject(_));
    EXPECT_CALL(*buyDecision6ConfigMock,  fromJsonObject(_));
    EXPECT_CALL(*buyDecision7ConfigMock,  fromJsonObject(_));
    EXPECT_CALL(*buyDecision8ConfigMock,  fromJsonObject(_));
    EXPECT_CALL(*sellDecision1ConfigMock, fromJsonObject(_));
    EXPECT_CALL(*sellDecision2ConfigMock, fromJsonObject(_));
    EXPECT_CALL(*sellDecision3ConfigMock, fromJsonObject(_));
    EXPECT_CALL(*sellDecision4ConfigMock, fromJsonObject(_));
    // clang-format on

    const QString content =
        R"({"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"b5":{"enabled":false},"b6":{"enabled":false},"b7":{"enabled":false},"b8":{"enabled":false},"s1":{"enabled":false},"s2":{"enabled":false},"s3":{"enabled":false},"s4":{"enabled":false}})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config->fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_DecisionMakerConfig, Test_toJsonString)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*buyDecision1ConfigMock,  toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*buyDecision2ConfigMock,  toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*buyDecision3ConfigMock,  toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*buyDecision4ConfigMock,  toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*buyDecision5ConfigMock,  toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*buyDecision6ConfigMock,  toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*buyDecision7ConfigMock,  toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*buyDecision8ConfigMock,  toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*sellDecision1ConfigMock, toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*sellDecision2ConfigMock, toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*sellDecision3ConfigMock, toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    EXPECT_CALL(*sellDecision4ConfigMock, toJsonString()).WillOnce(Return(R"({"enabled":false})"));
    // clang-format on

    const QString content = config->toJsonString();
    const QString expectedContent =
        R"({"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"b5":{"enabled":false},"b6":{"enabled":false},"b7":{"enabled":false},"b8":{"enabled":false},"s1":{"enabled":false},"s2":{"enabled":false},"s3":{"enabled":false},"s4":{"enabled":false}})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_DecisionMakerConfig, Test_variantsToJsonStringList)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*buyDecision1ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*buyDecision2ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*buyDecision3ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*buyDecision4ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*buyDecision5ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*buyDecision6ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*buyDecision7ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*buyDecision8ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*sellDecision1ConfigMock, variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*sellDecision2ConfigMock, variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*sellDecision3ConfigMock, variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*sellDecision4ConfigMock, variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    // clang-format on

    const QStringList variants         = config->variantsToJsonStringList();
    const QStringList expectedVariants = {
        "[\n{\"b1\":{\"enabled\":true},\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{"
        "\"enabled\":false},\"b6\":{\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":"
        "true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}},\n{\"b1\":{\"enabled\":true},"
        "\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{\"enabled\":false},\"b6\":{"
        "\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":false},\"s2\":{\"enabled\":"
        "true},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}}\n]",
        "[\n{\"b1\":{\"enabled\":false},\"b2\":{\"enabled\":true},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{"
        "\"enabled\":false},\"b6\":{\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":"
        "true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}},\n{\"b1\":{\"enabled\":false},"
        "\"b2\":{\"enabled\":true},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{\"enabled\":false},\"b6\":{"
        "\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":false},\"s2\":{\"enabled\":"
        "true},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}}\n]",
        "[\n{\"b1\":{\"enabled\":false},\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":true},\"b4\":{\"enabled\":false},\"b5\":{"
        "\"enabled\":false},\"b6\":{\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":"
        "true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}},\n{\"b1\":{\"enabled\":false},"
        "\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":true},\"b4\":{\"enabled\":false},\"b5\":{\"enabled\":false},\"b6\":{"
        "\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":false},\"s2\":{\"enabled\":"
        "true},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}}\n]",
        "[\n{\"b1\":{\"enabled\":false},\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":true},\"b5\":{"
        "\"enabled\":false},\"b6\":{\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":"
        "true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}},\n{\"b1\":{\"enabled\":false},"
        "\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":true},\"b5\":{\"enabled\":false},\"b6\":{"
        "\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":false},\"s2\":{\"enabled\":"
        "true},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}}\n]",
        "[\n{\"b1\":{\"enabled\":false},\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{"
        "\"enabled\":true},\"b6\":{\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":"
        "true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}},\n{\"b1\":{\"enabled\":false},"
        "\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{\"enabled\":true},\"b6\":{"
        "\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":false},\"s2\":{\"enabled\":"
        "true},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}}\n]",
        "[\n{\"b1\":{\"enabled\":false},\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{"
        "\"enabled\":false},\"b6\":{\"enabled\":true},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":"
        "true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}},\n{\"b1\":{\"enabled\":false},"
        "\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{\"enabled\":false},\"b6\":{"
        "\"enabled\":true},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":false},\"s2\":{\"enabled\":"
        "true},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}}\n]",
        "[\n{\"b1\":{\"enabled\":false},\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{"
        "\"enabled\":false},\"b6\":{\"enabled\":false},\"b7\":{\"enabled\":true},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":"
        "true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}},\n{\"b1\":{\"enabled\":false},"
        "\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{\"enabled\":false},\"b6\":{"
        "\"enabled\":false},\"b7\":{\"enabled\":true},\"b8\":{\"enabled\":false},\"s1\":{\"enabled\":false},\"s2\":{\"enabled\":"
        "true},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}}\n]",
        "[\n{\"b1\":{\"enabled\":false},\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{"
        "\"enabled\":false},\"b6\":{\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":true},\"s1\":{\"enabled\":"
        "true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}},\n{\"b1\":{\"enabled\":false},"
        "\"b2\":{\"enabled\":false},\"b3\":{\"enabled\":false},\"b4\":{\"enabled\":false},\"b5\":{\"enabled\":false},\"b6\":{"
        "\"enabled\":false},\"b7\":{\"enabled\":false},\"b8\":{\"enabled\":true},\"s1\":{\"enabled\":false},\"s2\":{\"enabled\":"
        "true},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}}\n]"
    };

    ASSERT_EQ(variants, expectedVariants);
}

TEST_F(Test_DecisionMakerConfig, Test_variantsToJsonStringListExtendedBySellDecisions)
{
    const InSequence seq;

    // clang-format off
    //EXPECT_CALL(*buyDecision1ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*buyDecision2ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*buyDecision3ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*buyDecision4ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*buyDecision5ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*buyDecision6ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*buyDecision7ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*buyDecision8ConfigMock,  variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*sellDecision1ConfigMock, variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    //EXPECT_CALL(*sellDecision2ConfigMock, variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*sellDecision3ConfigMock, variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    EXPECT_CALL(*sellDecision4ConfigMock, variantsAsJson()).WillOnce(Return(QStringList() << R"({"enabled":false})" << R"({"enabled":true})"));
    // clang-format on

    QStringList bestConfigs = {
        R"({"b1":{"enabled":true},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"b5":{"enabled":false},"b6":{"enabled":false},"b7":{"enabled":false},"b8":{"enabled":false},"s1":{"enabled":true},"s2":{"enabled":false},"s3":{"enabled":false},"s4":{"enabled":false}})",
        R"({"b1":{"enabled":false},"b2":{"enabled":true},"b3":{"enabled":false},"b4":{"enabled":false},"b5":{"enabled":false},"b6":{"enabled":false},"b7":{"enabled":false},"b8":{"enabled":false},"s1":{"enabled":false},"s2":{"enabled":true},"s3":{"enabled":false},"s4":{"enabled":false}})",
        R"({"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":true},"b4":{"enabled":false},"b5":{"enabled":false},"b6":{"enabled":false},"b7":{"enabled":false},"b8":{"enabled":false},"s1":{"enabled":true},"s2":{"enabled":false},"s3":{"enabled":false},"s4":{"enabled":false}})",
        R"({"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":true},"b5":{"enabled":false},"b6":{"enabled":false},"b7":{"enabled":false},"b8":{"enabled":false},"s1":{"enabled":false},"s2":{"enabled":true},"s3":{"enabled":false},"s4":{"enabled":false}})",
        R"({"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"b5":{"enabled":true},"b6":{"enabled":false},"b7":{"enabled":false},"b8":{"enabled":false},"s1":{"enabled":false},"s2":{"enabled":true},"s3":{"enabled":false},"s4":{"enabled":false}})",
        R"({"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"b5":{"enabled":false},"b6":{"enabled":true},"b7":{"enabled":false},"b8":{"enabled":false},"s1":{"enabled":false},"s2":{"enabled":true},"s3":{"enabled":false},"s4":{"enabled":false}})",
        R"({"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"b5":{"enabled":false},"b6":{"enabled":false},"b7":{"enabled":true},"b8":{"enabled":false},"s1":{"enabled":false},"s2":{"enabled":true},"s3":{"enabled":false},"s4":{"enabled":false}})",
        R"({"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"b5":{"enabled":false},"b6":{"enabled":false},"b7":{"enabled":false},"b8":{"enabled":true},"s1":{"enabled":false},"s2":{"enabled":true},"s3":{"enabled":false},"s4":{"enabled":false}})"
    };

    const QString variants = config->variantsToJsonStringListExtendedBySellDecisions(bestConfigs);
    const QString expectedVariants =
        "[\n{\"b1\":{\"enabled\":true},\"b2\":{\"enabled\":true},\"b3\":{\"enabled\":true},\"b4\":{\"enabled\":true},\"b5\":{"
        "\"enabled\":true},\"b6\":{\"enabled\":true},\"b7\":{\"enabled\":true},\"b8\":{\"enabled\":true},\"s1\":{\"enabled\":"
        "true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}}},\n{\"b1\":{\"enabled\":true},"
        "\"b2\":{\"enabled\":true},\"b3\":{\"enabled\":true},\"b4\":{\"enabled\":true},\"b5\":{\"enabled\":true},\"b6\":{"
        "\"enabled\":true},\"b7\":{\"enabled\":true},\"b8\":{\"enabled\":true},\"s1\":{\"enabled\":true},\"s2\":{\"enabled\":"
        "false},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":true}}},\n{\"b1\":{\"enabled\":true},\"b2\":{\"enabled\":true},"
        "\"b3\":{\"enabled\":true},\"b4\":{\"enabled\":true},\"b5\":{\"enabled\":true},\"b6\":{\"enabled\":true},\"b7\":{"
        "\"enabled\":true},\"b8\":{\"enabled\":true},\"s1\":{\"enabled\":true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":"
        "true},\"s4\":{\"enabled\":false}}},\n{\"b1\":{\"enabled\":true},\"b2\":{\"enabled\":true},\"b3\":{\"enabled\":true},"
        "\"b4\":{\"enabled\":true},\"b5\":{\"enabled\":true},\"b6\":{\"enabled\":true},\"b7\":{\"enabled\":true},\"b8\":{"
        "\"enabled\":true},\"s1\":{\"enabled\":true},\"s2\":{\"enabled\":false},\"s3\":{\"enabled\":true},\"s4\":{\"enabled\":"
        "true}}},\n{\"b1\":{\"enabled\":true},\"b2\":{\"enabled\":true},\"b3\":{\"enabled\":true},\"b4\":{\"enabled\":true},"
        "\"b5\":{\"enabled\":true},\"b6\":{\"enabled\":true},\"b7\":{\"enabled\":true},\"b8\":{\"enabled\":true},\"s1\":{"
        "\"enabled\":false},\"s2\":{\"enabled\":true},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":false}}},\n{\"b1\":{"
        "\"enabled\":true},\"b2\":{\"enabled\":true},\"b3\":{\"enabled\":true},\"b4\":{\"enabled\":true},\"b5\":{\"enabled\":"
        "true},\"b6\":{\"enabled\":true},\"b7\":{\"enabled\":true},\"b8\":{\"enabled\":true},\"s1\":{\"enabled\":false},\"s2\":{"
        "\"enabled\":true},\"s3\":{\"enabled\":false},\"s4\":{\"enabled\":true}}},\n{\"b1\":{\"enabled\":true},\"b2\":{"
        "\"enabled\":true},\"b3\":{\"enabled\":true},\"b4\":{\"enabled\":true},\"b5\":{\"enabled\":true},\"b6\":{\"enabled\":"
        "true},\"b7\":{\"enabled\":true},\"b8\":{\"enabled\":true},\"s1\":{\"enabled\":false},\"s2\":{\"enabled\":true},\"s3\":{"
        "\"enabled\":true},\"s4\":{\"enabled\":false}}},\n{\"b1\":{\"enabled\":true},\"b2\":{\"enabled\":true},\"b3\":{"
        "\"enabled\":true},\"b4\":{\"enabled\":true},\"b5\":{\"enabled\":true},\"b6\":{\"enabled\":true},\"b7\":{\"enabled\":"
        "true},\"b8\":{\"enabled\":true},\"s1\":{\"enabled\":false},\"s2\":{\"enabled\":true},\"s3\":{\"enabled\":true},\"s4\":{"
        "\"enabled\":true}}}\n]";

    ASSERT_EQ(variants, expectedVariants);
}

TEST_F(Test_DecisionMakerConfig, Test_getBuyDecision1Config)
{
    ASSERT_EQ(config->getBuyDecision1Config(), buyDecision1ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getBuyDecision2Config)
{
    ASSERT_EQ(config->getBuyDecision2Config(), buyDecision2ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getBuyDecision3Config)
{
    ASSERT_EQ(config->getBuyDecision3Config(), buyDecision3ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getBuyDecision4Config)
{
    ASSERT_EQ(config->getBuyDecision4Config(), buyDecision4ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getBuyDecision5Config)
{
    ASSERT_EQ(config->getBuyDecision5Config(), buyDecision5ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getBuyDecision6Config)
{
    ASSERT_EQ(config->getBuyDecision6Config(), buyDecision6ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getBuyDecision7Config)
{
    ASSERT_EQ(config->getBuyDecision7Config(), buyDecision7ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getBuyDecision8Config)
{
    ASSERT_EQ(config->getBuyDecision8Config(), buyDecision8ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getSellDecision1Config)
{
    ASSERT_EQ(config->getSellDecision1Config(), sellDecision1ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getSellDecision2Config)
{
    ASSERT_EQ(config->getSellDecision2Config(), sellDecision2ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getSellDecision3Config)
{
    ASSERT_EQ(config->getSellDecision3Config(), sellDecision3ConfigMock);
}

TEST_F(Test_DecisionMakerConfig, Test_getSellDecision4Config)
{
    ASSERT_EQ(config->getSellDecision4Config(), sellDecision4ConfigMock);
}
