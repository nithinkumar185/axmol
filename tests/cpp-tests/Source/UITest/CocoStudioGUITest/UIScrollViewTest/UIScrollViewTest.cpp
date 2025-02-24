/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 https://axmol.dev/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "UIScrollViewTest.h"

USING_NS_AX;
using namespace ax::ui;

UIScrollViewTests::UIScrollViewTests()
{
    ADD_TEST_CASE(UIScrollViewTest_Vertical);
    ADD_TEST_CASE(UIScrollViewTest_Horizontal);
    ADD_TEST_CASE(UIScrollViewTest_Both);
    ADD_TEST_CASE(UIScrollViewTest_ScrollToPercentBothDirection);
    ADD_TEST_CASE(UIScrollViewTest_ScrollToPercentBothDirection_Bounce);
    ADD_TEST_CASE(UIScrollViewNestTest);
    ADD_TEST_CASE(UIScrollViewRotated);
    ADD_TEST_CASE(UIScrollViewDisableTest);
    ADD_TEST_CASE(UIScrollViewInnerSize);
    ADD_TEST_CASE(UIScrollViewTestEvents);
    ADD_TEST_CASE(UIScrollViewStopScrollingTest);
}
// UIScrollViewTest_Vertical

UIScrollViewTest_Vertical::UIScrollViewTest_Vertical() : _displayValueLabel(nullptr) {}

UIScrollViewTest_Vertical::~UIScrollViewTest_Vertical() {}

bool UIScrollViewTest_Vertical::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the scrollview alert will be displayed
        _displayValueLabel = Text::create("Move by vertical direction", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        // Add the alert
        Text* alert = Text::create("ScrollView vertical", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the scrollview by vertical
        ui::ScrollView* scrollView = ui::ScrollView::create();
        scrollView->setContentSize(Size(280.0f, 100.0f));
        Size backgroundSize = background->getContentSize();
        scrollView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                         (backgroundSize.width - scrollView->getContentSize().width) / 2.0f,
                                     (widgetSize.height - backgroundSize.height) / 2.0f +
                                         (backgroundSize.height - scrollView->getContentSize().height) / 2.0f));
        scrollView->setScrollBarWidth(4);
        scrollView->setScrollBarPositionFromCorner(Vec2(2, 2));
        scrollView->setScrollBarColor(Color3B::WHITE);
        scrollView->setGlobalZOrder(200);
        _uiLayer->addChild(scrollView);

        ImageView* imageView = ImageView::create("cocosui/ccicon.png");

        float innerWidth  = scrollView->getContentSize().width;
        float innerHeight = scrollView->getContentSize().height + imageView->getContentSize().height;

        scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

        Button* button = Button::create("cocosui/animationbuttonnormal.png", "cocosui/animationbuttonpressed.png");
        button->setPosition(Vec2(innerWidth / 2.0f,
                                 scrollView->getInnerContainerSize().height - button->getContentSize().height / 2.0f));
        scrollView->addChild(button);

        Button* titleButton = Button::create("cocosui/backtotopnormal.png", "cocosui/backtotoppressed.png");
        titleButton->setTitleText("Title Button");
        titleButton->setPosition(
            Vec2(innerWidth / 2.0f, button->getBottomBoundary() - button->getContentSize().height));
        scrollView->addChild(titleButton);

        Button* button_scale9 = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        button_scale9->setScale9Enabled(true);
        button_scale9->setContentSize(Size(100.0f, button_scale9->getVirtualRendererSize().height));
        button_scale9->setPosition(
            Vec2(innerWidth / 2.0f, titleButton->getBottomBoundary() - titleButton->getContentSize().height));
        scrollView->addChild(button_scale9);

        imageView->setPosition(Vec2(innerWidth / 2.0f, imageView->getContentSize().height / 2.0f));
        scrollView->addChild(imageView);
        scrollView->scrollToBottom(2.0, true);
        return true;
    }

    return false;
}

// UIScrollViewTest_Horizontal

UIScrollViewTest_Horizontal::UIScrollViewTest_Horizontal() : _displayValueLabel(nullptr) {}

UIScrollViewTest_Horizontal::~UIScrollViewTest_Horizontal() {}

bool UIScrollViewTest_Horizontal::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the scrollview alert will be displayed
        _displayValueLabel = Text::create("Move by horizontal direction", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        Text* alert = Text::create("ScrollView horizontal", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the scrollview by horizontal
        ui::ScrollView* scrollView = ui::ScrollView::create();
        scrollView->setBounceEnabled(true);
        scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        scrollView->setContentSize(Size(280.0f, 150.0f));
        scrollView->setInnerContainerSize(scrollView->getContentSize());
        scrollView->setScrollBarPositionFromCorner(Vec2(4, 4));
        scrollView->setScrollBarColor(Color3B::YELLOW);
        Size backgroundSize = background->getContentSize();
        scrollView->setPosition((widgetSize - scrollView->getContentSize()) / 2.0f);
        _uiLayer->addChild(scrollView);

        ImageView* imageView = ImageView::create("cocosui/ccicon.png");

        float innerWidth  = scrollView->getContentSize().width + imageView->getContentSize().width;
        float innerHeight = scrollView->getContentSize().height;

        scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

        Button* button = Button::create("cocosui/animationbuttonnormal.png", "cocosui/animationbuttonpressed.png");
        button->setPosition(Vec2(button->getContentSize().width / 2.0f,
                                 scrollView->getInnerContainerSize().height - button->getContentSize().height / 2.0f));
        scrollView->addChild(button);

        Button* titleButton = Button::create("cocosui/backtotopnormal.png", "cocosui/backtotoppressed.png");
        titleButton->setTitleText("Title Button");
        titleButton->setPosition(Vec2(button->getRightBoundary() + button->getContentSize().width / 2.0f,
                                      button->getBottomBoundary() - button->getContentSize().height / 2.0f));
        scrollView->addChild(titleButton);

        Button* button_scale9 = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        button_scale9->setScale9Enabled(true);
        button_scale9->setContentSize(Size(100.0f, button_scale9->getVirtualRendererSize().height));
        button_scale9->setPosition(
            Vec2(titleButton->getRightBoundary() + titleButton->getContentSize().width / 2.0f,
                 titleButton->getBottomBoundary() - titleButton->getContentSize().height / 2.0f));
        scrollView->addChild(button_scale9);

        imageView->setPosition(
            Vec2(innerWidth - imageView->getContentSize().width / 2.0f,
                 button_scale9->getBottomBoundary() - button_scale9->getContentSize().height / 2.0f));
        scrollView->addChild(imageView);

        // Jump to right
        scrollView->jumpToRight();

        return true;
    }

    return false;
}

// UIScrollViewTest_Both
UIScrollViewTest_Both::UIScrollViewTest_Both() : _displayValueLabel(nullptr) {}

UIScrollViewTest_Both::~UIScrollViewTest_Both() {}

bool UIScrollViewTest_Both::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("Move by any direction", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        // Add the alert
        Text* alert = Text::create("ScrollView both", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = static_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the dragpanel
        ui::ScrollView* scrollView = ui::ScrollView::create();
        scrollView->setDirection(ui::ScrollView::Direction::BOTH);
        scrollView->setTouchEnabled(true);
        scrollView->setBounceEnabled(true);
        scrollView->setBackGroundImageScale9Enabled(true);
        scrollView->setBackGroundImage("cocosui/green_edit.png");
        scrollView->setContentSize(Size(210, 122.5));
        scrollView->setScrollBarWidth(4);
        scrollView->setScrollBarPositionFromCorner(Vec2(6, 6));
        Size backgroundSize = background->getContentSize();
        scrollView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                         (backgroundSize.width - scrollView->getContentSize().width) / 2.0f,
                                     (widgetSize.height - backgroundSize.height) / 2.0f +
                                         (backgroundSize.height - scrollView->getContentSize().height) / 2.0f));
        ImageView* imageView = ImageView::create("Hello.png");
        scrollView->addChild(imageView);

        scrollView->setInnerContainerSize(imageView->getContentSize());
        Size innerSize = scrollView->getInnerContainerSize();
        imageView->setPosition(Vec2(innerSize.width / 2.0f, innerSize.height / 2.0f));

        _uiLayer->addChild(scrollView);

        // Jump to right bottom
        scrollView->jumpToBottomRight();

        return true;
    }

    return false;
}

// UIScrollViewTest_ScrollToPercentBothDirection
UIScrollViewTest_ScrollToPercentBothDirection::UIScrollViewTest_ScrollToPercentBothDirection()
    : _displayValueLabel(nullptr)
{}

UIScrollViewTest_ScrollToPercentBothDirection::~UIScrollViewTest_ScrollToPercentBothDirection() {}

bool UIScrollViewTest_ScrollToPercentBothDirection::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("No Event", "fonts/Marker Felt.ttf", 30);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        // Add the alert
        Text* alert =
            Text::create("ScrollView scroll to percent both direction without scroll bar", "fonts/Marker Felt.ttf", 20);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 4.5));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = static_cast<Layout*>(root->getChildByName("background_Panel"));

        ui::ScrollView* sc = ui::ScrollView::create();
        sc->setBackGroundColor(Color3B::GREEN);
        sc->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        sc->setDirection(ui::ScrollView::Direction::BOTH);
        sc->setInnerContainerSize(Size(480.0f, 320.0f));
        sc->setContentSize(Size(100.0f, 100.0f));
        sc->setScrollBarEnabled(false);
        Size backgroundSize = background->getContentSize();
        sc->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                 (backgroundSize.width - sc->getContentSize().width) / 2.0f,
                             (widgetSize.height - backgroundSize.height) / 2.0f +
                                 (backgroundSize.height - sc->getContentSize().height) / 2.0f));
        sc->scrollToPercentBothDirection(Vec2(50, 50), 1, true);
        ImageView* iv = ImageView::create("cocosui/Hello.png");
        iv->setPosition(Vec2(240.0f, 160.0f));
        sc->addChild(iv);
        _uiLayer->addChild(sc);

        return true;
    }
    return false;
}

// UIScrollViewTest_ScrollToPercentBothDirection_Bounce
UIScrollViewTest_ScrollToPercentBothDirection_Bounce::UIScrollViewTest_ScrollToPercentBothDirection_Bounce()
    : _displayValueLabel(nullptr)
{}

UIScrollViewTest_ScrollToPercentBothDirection_Bounce::~UIScrollViewTest_ScrollToPercentBothDirection_Bounce() {}

bool UIScrollViewTest_ScrollToPercentBothDirection_Bounce::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("No Event", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        // Add the alert
        Text* alert = Text::create("ScrollView scroll to percent both direction bounce", "fonts/Marker Felt.ttf", 20);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 4.5));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        ui::ScrollView* sc = ui::ScrollView::create();
        sc->setBackGroundColor(Color3B::GREEN);
        sc->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        sc->setBounceEnabled(true);
        sc->setDirection(ui::ScrollView::Direction::BOTH);
        sc->setInnerContainerSize(Size(480.0f, 320.0f));
        sc->setContentSize(Size(100.0f, 100.0f));
        sc->setScrollBarPositionFromCornerForHorizontal(Vec2(5.0f, sc->getContentSize().height - 5));
        sc->setScrollBarPositionFromCornerForVertical(Vec2(sc->getContentSize().width - 5, 5.0f));
        sc->setScrollBarAutoHideEnabled(false);
        Size backgroundSize = background->getContentSize();
        sc->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                 (backgroundSize.width - sc->getContentSize().width) / 2.0f,
                             (widgetSize.height - backgroundSize.height) / 2.0f +
                                 (backgroundSize.height - sc->getContentSize().height) / 2.0f));
        sc->scrollToPercentBothDirection(Vec2(50.0f, 50.0f), 1, true);
        ImageView* iv = ImageView::create("cocosui/Hello.png");
        iv->setPosition(Vec2(240.0f, 160.0f));
        sc->addChild(iv);
        _uiLayer->addChild(sc);
        return true;
    }

    return false;
}

// UIScrollViewNestTest

UIScrollViewNestTest::UIScrollViewNestTest() : _displayValueLabel(nullptr) {}

UIScrollViewNestTest::~UIScrollViewNestTest() {}

bool UIScrollViewNestTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the scrollview alert will be displayed
        _displayValueLabel = Text::create("Move by vertical direction", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        // Add the alert
        Text* alert = Text::create("ScrollView vertical", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the scrollview by vertical
        ui::ScrollView* scrollView = ui::ScrollView::create();
        scrollView->setContentSize(Size(280.0f, 150.0f));
        scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
        scrollView->setScrollBarPositionFromCorner(Vec2(4, 4));
        Size backgroundSize = background->getContentSize();
        scrollView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                         (backgroundSize.width - scrollView->getContentSize().width) / 2.0f,
                                     (widgetSize.height - backgroundSize.height) / 2.0f +
                                         (backgroundSize.height - scrollView->getContentSize().height) / 2.0f));
        scrollView->setScrollBarPositionFromCornerForVertical(Vec2(scrollView->getContentSize().width - 4, 4.0f));
        scrollView->setScrollBarColor(Color3B::BLUE);
        _uiLayer->addChild(scrollView);

        ImageView* imageView = ImageView::create("cocosui/ccicon.png");

        float innerWidth  = scrollView->getContentSize().width;
        float innerHeight = scrollView->getContentSize().height + imageView->getContentSize().height;

        scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

        Button* button = Button::create("cocosui/animationbuttonnormal.png", "cocosui/animationbuttonpressed.png");
        button->setPosition(Vec2(innerWidth / 2.0f,
                                 scrollView->getInnerContainerSize().height - button->getContentSize().height / 2.0f));
        scrollView->addChild(button);

        Button* titleButton = Button::create("cocosui/backtotopnormal.png", "cocosui/backtotoppressed.png");
        titleButton->setTitleText("Title Button");
        titleButton->setPosition(
            Vec2(innerWidth / 2.0f, button->getBottomBoundary() - button->getContentSize().height));
        scrollView->addChild(titleButton);

        Button* button_scale9 = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        button_scale9->setScale9Enabled(true);
        button_scale9->setContentSize(Size(100.0f, button_scale9->getVirtualRendererSize().height));
        button_scale9->setPosition(
            Vec2(innerWidth / 2.0f, titleButton->getBottomBoundary() - titleButton->getContentSize().height));
        scrollView->addChild(button_scale9);

        imageView->setPosition(Vec2(innerWidth / 2.0f, imageView->getContentSize().height / 2.0f));
        scrollView->addChild(imageView);

        // Create the scrollview by both
        ui::ScrollView* sc = ui::ScrollView::create();
        sc->setBackGroundColor(Color3B::GREEN);
        sc->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        sc->setBounceEnabled(true);
        sc->setDirection(ui::ScrollView::Direction::BOTH);
        sc->setInnerContainerSize(Size(480.0f, 320.0f));
        sc->setContentSize(Size(100.0f, 100.0f));
        sc->setPropagateTouchEvents(false);
        sc->setPosition(Vec2(180.0f, 100.0f));
        sc->scrollToPercentBothDirection(Vec2(50.0f, 50.0f), 1, true);
        sc->setScrollBarPositionFromCorner(Vec2(4.0f, 4.0f));
        ImageView* iv = ImageView::create("cocosui/Hello.png");
        iv->setPosition(Vec2(240.0f, 160.0f));
        sc->addChild(iv);

        scrollView->addChild(sc);

        return true;
    }

    return false;
}

// UIScrollViewRotated

UIScrollViewRotated::UIScrollViewRotated() : _displayValueLabel(nullptr) {}

UIScrollViewRotated::~UIScrollViewRotated() {}

bool UIScrollViewRotated::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the scrollview alert will be displayed
        _displayValueLabel = Text::create("Move by vertical direction", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        // Add the alert
        Text* alert = Text::create("ScrollView vertical", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the scrollview by vertical
        ui::ScrollView* scrollView = ui::ScrollView::create();
        scrollView->setContentSize(Size(280.0f, 150.0f));
        scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
        Size backgroundSize = background->getContentSize();
        scrollView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                         (backgroundSize.width - scrollView->getContentSize().width) / 2.0f,
                                     (widgetSize.height - backgroundSize.height) / 2.0f +
                                         (backgroundSize.height - scrollView->getContentSize().height) / 2.0f + 100));
        scrollView->setRotation(45);
        scrollView->setScrollBarPositionFromCorner(Vec2(4, 4));
        scrollView->setScrollBarColor(Color3B::WHITE);
        _uiLayer->addChild(scrollView);

        ImageView* imageView = ImageView::create("cocosui/ccicon.png");

        float innerWidth  = scrollView->getContentSize().width;
        float innerHeight = scrollView->getContentSize().height + imageView->getContentSize().height;

        scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

        Button* button = Button::create("cocosui/animationbuttonnormal.png", "cocosui/animationbuttonpressed.png");
        button->setPosition(Vec2(innerWidth / 2.0f,
                                 scrollView->getInnerContainerSize().height - button->getContentSize().height / 2.0f));
        scrollView->addChild(button);

        Button* titleButton = Button::create("cocosui/backtotopnormal.png", "cocosui/backtotoppressed.png");
        titleButton->setTitleText("Title Button");
        titleButton->setPosition(
            Vec2(innerWidth / 2.0f, button->getBottomBoundary() - button->getContentSize().height));
        scrollView->addChild(titleButton);

        Button* button_scale9 = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        button_scale9->setScale9Enabled(true);
        button_scale9->setContentSize(Size(100.0f, button_scale9->getVirtualRendererSize().height));
        button_scale9->setPosition(
            Vec2(innerWidth / 2.0f, titleButton->getBottomBoundary() - titleButton->getContentSize().height));
        scrollView->addChild(button_scale9);

        imageView->setPosition(Vec2(innerWidth / 2.0f, imageView->getContentSize().height / 2.0f));
        scrollView->addChild(imageView);

        return true;
    }

    return false;
}

// UIScrollViewDisableTest

UIScrollViewDisableTest::UIScrollViewDisableTest() : _displayValueLabel(nullptr) {}

UIScrollViewDisableTest::~UIScrollViewDisableTest() {}

bool UIScrollViewDisableTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the scrollview alert will be displayed
        _displayValueLabel = Text::create("ScrollView Disable Test", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        // Add the alert
        Text* alert = Text::create("ScrollView vertical", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the scrollview by vertical
        ui::ScrollView* scrollView = ui::ScrollView::create();
        scrollView->setContentSize(Size(280.0f, 100.0f));
        Size backgroundSize = background->getContentSize();
        scrollView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                         (backgroundSize.width - scrollView->getContentSize().width) / 2.0f,
                                     (widgetSize.height - backgroundSize.height) / 2.0f +
                                         (backgroundSize.height - scrollView->getContentSize().height) / 2.0f));
        scrollView->setScrollBarWidth(4);
        scrollView->setTouchEnabled(false);
        scrollView->setScrollBarPositionFromCorner(Vec2(2, 2));
        scrollView->setScrollBarColor(Color3B::WHITE);
        _uiLayer->addChild(scrollView);

        ImageView* imageView = ImageView::create("cocosui/ccicon.png");

        float innerWidth  = scrollView->getContentSize().width;
        float innerHeight = scrollView->getContentSize().height + imageView->getContentSize().height;

        scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

        Button* button = Button::create("cocosui/animationbuttonnormal.png", "cocosui/animationbuttonpressed.png");
        button->setPosition(Vec2(innerWidth / 2.0f,
                                 scrollView->getInnerContainerSize().height - button->getContentSize().height / 2.0f));
        scrollView->addChild(button);

        Button* titleButton = Button::create("cocosui/backtotopnormal.png", "cocosui/backtotoppressed.png");
        titleButton->setTitleText("Title Button");
        titleButton->setPosition(
            Vec2(innerWidth / 2.0f, button->getBottomBoundary() - button->getContentSize().height));
        scrollView->addChild(titleButton);

        Button* button_scale9 = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        button_scale9->setScale9Enabled(true);
        button_scale9->setContentSize(Size(100.0f, button_scale9->getVirtualRendererSize().height));
        button_scale9->setPosition(
            Vec2(innerWidth / 2.0f, titleButton->getBottomBoundary() - titleButton->getContentSize().height));
        scrollView->addChild(button_scale9);

        imageView->setPosition(Vec2(innerWidth / 2.0f, imageView->getContentSize().height / 2.0f));
        scrollView->addChild(imageView);

        return true;
    }

    return false;
}

// UIScrollViewInnerSize

UIScrollViewInnerSize::UIScrollViewInnerSize() : _displayValueLabel(nullptr) {}

UIScrollViewInnerSize::~UIScrollViewInnerSize() {}

bool UIScrollViewInnerSize::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the scrollview alert will be displayed
        _displayValueLabel = Text::create("ScrollView InnerSize Test", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        // Add the alert
        Text* alert = Text::create("ScrollView & InnerContainer should be align to the left and top point",
                                   "fonts/Marker Felt.ttf", 10);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the scrollview by vertical
        ui::ScrollView* scrollView = ui::ScrollView::create();
        scrollView->setContentSize(Size(200.0f, 50.0f));
        Size backgroundSize = background->getContentSize();
        scrollView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                         (backgroundSize.width - scrollView->getContentSize().width) / 2.0f,
                                     (widgetSize.height - backgroundSize.height) / 2.0f +
                                         (backgroundSize.height - scrollView->getContentSize().height) / 2.0f));
        scrollView->setScrollBarWidth(4);
        scrollView->setTouchEnabled(false);
        scrollView->setScrollBarPositionFromCorner(Vec2(2, 2));
        scrollView->setScrollBarColor(Color3B::WHITE);
        scrollView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        scrollView->setBackGroundColor(Color3B(150, 150, 100));
        scrollView->getInnerContainer()->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        scrollView->getInnerContainer()->setBackGroundColor(Color3B(50, 100, 50));
        scrollView->getInnerContainer()->setBackGroundColorOpacity(100);
        scrollView->setClippingEnabled(false);
        _uiLayer->addChild(scrollView);

        scrollView->setInnerContainerSize(Size(250.f, 100.f));

        return true;
    }

    return false;
}

// UIScrollViewTestEvents

UIScrollViewTestEvents::UIScrollViewTestEvents() : _displayValueLabel(nullptr) {}

bool UIScrollViewTestEvents::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("(no events)", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        // Add the alert
        Text* alert = Text::create("ScrollView events", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = static_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the dragpanel
        ui::ScrollView* scrollView = ui::ScrollView::create();
        scrollView->setDirection(ui::ScrollView::Direction::BOTH);
        scrollView->setTouchEnabled(true);
        scrollView->setBounceEnabled(true);
        scrollView->setBackGroundImageScale9Enabled(true);
        scrollView->setBackGroundImage("cocosui/green_edit.png");
        scrollView->setContentSize(Size(210, 122.5));
        scrollView->setScrollBarWidth(4);
        scrollView->setScrollBarPositionFromCorner(Vec2(6, 6));
        Size backgroundSize = background->getContentSize();
        scrollView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                         (backgroundSize.width - scrollView->getContentSize().width) / 2.0f,
                                     (widgetSize.height - backgroundSize.height) / 2.0f +
                                         (backgroundSize.height - scrollView->getContentSize().height) / 2.0f));
        ImageView* imageView = ImageView::create("Hello.png");
        scrollView->addChild(imageView);

        scrollView->setInnerContainerSize(imageView->getContentSize());
        Size innerSize = scrollView->getInnerContainerSize();
        imageView->setPosition(Vec2(innerSize.width / 2.0f, innerSize.height / 2.0f));

        _uiLayer->addChild(scrollView);

        // Jump to right bottom
        scrollView->jumpToBottomRight();

        auto getRandomColor = [] { return Color4B(random(0, 255), random(0, 255), random(0, 255), 255); };
        scrollView->addEventListener([&](Object*, ui::ScrollView::EventType e) {
            switch (e)
            {
            case ui::ScrollView::EventType::SCROLLING_BEGAN:
                _displayValueLabel->setString("scrolling began!");
                _displayValueLabel->setTextColor(getRandomColor());
                break;
            case ui::ScrollView::EventType::SCROLLING_ENDED:
                _displayValueLabel->setString("scrolling ended!");
                _displayValueLabel->setTextColor(getRandomColor());
                break;
            default:
                break;
            }
        });

        return true;
    }

    return false;
}

// UIScrollViewStopScrollingTest

UIScrollViewStopScrollingTest::UIScrollViewStopScrollingTest()
    : _displayValueLabel(nullptr), _scrollView(nullptr), _remainingTime(0.0f)
{}

bool UIScrollViewStopScrollingTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the time remaining till scrolling stop will be displayed.
        _displayValueLabel = Text::create("Scrolling stop isn't scheduled", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5f));
        _uiLayer->addChild(_displayValueLabel);

        // Add the alert
        Text* alert = Text::create("Click the button and start to scroll", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = static_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the dragpanel
        _scrollView = ui::ScrollView::create();
        _scrollView->setDirection(ui::ScrollView::Direction::BOTH);
        _scrollView->setTouchEnabled(true);
        _scrollView->setBounceEnabled(true);
        _scrollView->setBackGroundImageScale9Enabled(true);
        _scrollView->setBackGroundImage("cocosui/green_edit.png");
        _scrollView->setContentSize(Size(210, 122.5));
        _scrollView->setScrollBarWidth(4);
        _scrollView->setScrollBarPositionFromCorner(Vec2(6, 6));
        Size backgroundSize = background->getContentSize();
        _scrollView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                          (backgroundSize.width - _scrollView->getContentSize().width) / 2.0f,
                                      (widgetSize.height - backgroundSize.height) / 2.0f +
                                          (backgroundSize.height - _scrollView->getContentSize().height) / 2.0f));
        ImageView* imageView = ImageView::create("Hello.png");
        _scrollView->addChild(imageView);
        _scrollView->setInnerContainerSize(imageView->getContentSize());
        Size innerSize = _scrollView->getInnerContainerSize();
        imageView->setPosition(Vec2(innerSize.width / 2.0f, innerSize.height / 2.0f));
        _uiLayer->addChild(_scrollView);

        // Log some ScrollView events.
        _scrollView->addEventListener([&](Object*, ui::ScrollView::EventType e) {
            switch (e)
            {
            case ui::ScrollView::EventType::SCROLLING_BEGAN:
                AXLOG("scrolling began!");
                break;
            case ui::ScrollView::EventType::SCROLLING_ENDED:
                AXLOG("scrolling ended!");
                break;
            case ui::ScrollView::EventType::AUTOSCROLL_ENDED:
                AXLOG("auto-scrolling ended!");
                break;
            default:
                break;
            }
        });

        // Jump to right bottom
        _scrollView->jumpToBottomRight();

        // Add button that will schedule scrolling stop when it is clicked.
        Button* button_scale9 = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        button_scale9->setTitleText("Stop scrolling in 3 sec.");
        button_scale9->setScale9Enabled(true);
        button_scale9->setContentSize(Size(120.0f, button_scale9->getVirtualRendererSize().height));
        button_scale9->setPosition(Vec2(innerSize.width / 2.0f, innerSize.height / 2.0f));
        button_scale9->addClickEventListener([this](Object*) { this->_remainingTime = 3.0f; });
        _scrollView->addChild(button_scale9);

        // Schedule update for this scene.
        Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);

        return true;
    }

    return false;
}

void UIScrollViewStopScrollingTest::update(float dt)
{
    UIScene::update(dt);

    if (_remainingTime > 0.0f)
    {
        _remainingTime -= dt;

        if (_remainingTime > 0.0f)
        {
            // Update timer caption.
            char strRemainingTime[100];
            sprintf(strRemainingTime, "Stop scrolling in %.1f sec.", _remainingTime);
            _displayValueLabel->setString(std::string(strRemainingTime));
        }
        else
        {
            _scrollView->stopOverallScroll();

            // Update timer caption.
            std::string strRemainingTime = "Scrolling stop isn't scheduled";
            _displayValueLabel->setString(strRemainingTime);

            // Show hint label.
            auto hintLabel   = Label::createWithTTF("Stopped!", "fonts/Marker Felt.ttf", 32.0f);
            Size contentSize = _uiLayer->getContentSize();
            hintLabel->setPosition(Vec2(contentSize.width / 2.0f, contentSize.height / 2.0f));
            hintLabel->runAction(Spawn::createWithTwoActions(FadeOut::create(0.3f), ScaleTo::create(0.3f, 2.0f)));
            _uiLayer->addChild(hintLabel);
        }
    }
}
