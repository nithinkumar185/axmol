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

#include "RenderTextureTest.h"

USING_NS_AX;
using namespace ax::ui;

RenderTextureTests::RenderTextureTests()
{
    ADD_TEST_CASE(RenderTextureSave);
    ADD_TEST_CASE(RenderTextureIssue937);
    ADD_TEST_CASE(RenderTextureZbuffer);
    ADD_TEST_CASE(RenderTextureTestDepthStencil);
    ADD_TEST_CASE(RenderTextureTargetNode);
    ADD_TEST_CASE(SpriteRenderTextureBug);
    ADD_TEST_CASE(RenderTexturePartTest);
    ADD_TEST_CASE(Issue16113Test);
};

/**
 * Implementation of RenderTextureSave
 */
RenderTextureSave::RenderTextureSave()
{
    auto s = Director::getInstance()->getWinSize();

    // create a render texture, this is what we are going to draw into
    _target = RenderTexture::create(s.width, s.height, backend::PixelFormat::RGBA8);
    _target->retain();
    _target->setPosition(Vec2(s.width / 2, s.height / 2));

    // note that the render texture is a Node, and contains a sprite of its texture for convenience,
    // so we can just parent it to the scene like any other Node
    this->addChild(_target, -1);

    auto listener            = EventListenerTouchAllAtOnce::create();
    listener->onTouchesMoved = AX_CALLBACK_2(RenderTextureSave::onTouchesMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // Save Image menu
    MenuItemFont::setFontSize(16);
    auto item1 =
        MenuItemFont::create("Save Image PMA", AX_CALLBACK_1(RenderTextureSave::saveImageWithPremultipliedAlpha, this));
    auto item2 = MenuItemFont::create("Save Image Non-PMA",
                                      AX_CALLBACK_1(RenderTextureSave::saveImageWithNonPremultipliedAlpha, this));
    auto item3 = MenuItemFont::create("Add Image", AX_CALLBACK_1(RenderTextureSave::addImage, this));
    auto item4 = MenuItemFont::create("Clear to Random", AX_CALLBACK_1(RenderTextureSave::clearImage, this));
    auto item5 =
        MenuItemFont::create("Clear to Transparent", AX_CALLBACK_1(RenderTextureSave::clearImageTransparent, this));
    auto menu = Menu::create(item1, item2, item3, item4, item5, nullptr);
    this->addChild(menu);
    menu->alignItemsVertically();
    menu->setPosition(Vec2(VisibleRect::rightTop().x - 80, VisibleRect::rightTop().y - 100));
}

std::string RenderTextureSave::title() const
{
    return "Touch the screen";
}

std::string RenderTextureSave::subtitle() const
{
    return "Press 'Save Image' to create an snapshot of the render texture";
}

void RenderTextureSave::clearImage(ax::Object* sender)
{
    _target->clear(AXRANDOM_0_1(), AXRANDOM_0_1(), AXRANDOM_0_1(), AXRANDOM_0_1());
}

void RenderTextureSave::clearImageTransparent(ax::Object* sender)
{
    _target->clear(0, 0, 0, 0);
}

void RenderTextureSave::saveImageWithPremultipliedAlpha(ax::Object* sender)
{
    static int counter = 0;

    char png[20];
    sprintf(png, "image-pma-%d.png", counter);

    auto callback = [this](RenderTexture* rt, std::string_view path) {
        auto sprite = Sprite::create(path);
        addChild(sprite);
        sprite->setScale(0.3f);
        sprite->setPosition(Vec2(40.0f, 40.0f));
        sprite->setRotation(counter * 3);
        _target->release();
    };

    _target->retain();
    _target->saveToFile(png, Image::Format::PNG, true, callback);
    // Add this function to avoid crash if we switch to a new scene.
    Director::getInstance()->getRenderer()->render();
    AXLOG("Image saved %s", png);

    counter++;
}

void RenderTextureSave::saveImageWithNonPremultipliedAlpha(ax::Object* sender)
{
    static int counter = 0;

    char png[20];
    sprintf(png, "image-no-pma-%d.png", counter);

    auto callback = [this](RenderTexture* rt, std::string_view path) {
        auto sprite = Sprite::create(path);
        addChild(sprite);
        sprite->setScale(0.3f);
        sprite->setPosition(Vec2(40.0f, 40.0f));
        sprite->setRotation(counter * 3);
        rt->release();
    };

    _target->retain();
    _target->saveToFileAsNonPMA(png, Image::Format::PNG, true, callback);

    // Add this function to avoid crash if we switch to a new scene.
    Director::getInstance()->getRenderer()->render();
    AXLOG("Image saved %s", png);

    counter++;
}

void RenderTextureSave::addImage(ax::Object* sender)
{
    auto s = Director::getInstance()->getWinSize();

    // begin drawing to the render texture
    _target->begin();

    Sprite* sprite = Sprite::create("Images/test-rgba1.png");
    sprite->setPosition(
        sprite->getContentSize().width + AXRANDOM_0_1() * (s.width - sprite->getContentSize().width),
        sprite->getContentSize().height + AXRANDOM_0_1() * (s.height - sprite->getContentSize().height));
    sprite->visit();

    // finish drawing and return context back to the screen
    _target->end();
}

RenderTextureSave::~RenderTextureSave()
{
    _target->release();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void RenderTextureSave::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    auto touch = touches[0];
    auto start = touch->getLocation();
    auto end   = touch->getPreviousLocation();

    // begin drawing to the render texture
    _target->begin();

    // for extra points, we'll draw this smoothly from the last position and vary the sprite's
    // scale/rotation/offset
    float distance = start.getDistance(end);
    if (distance > 1)
    {
        int d = (int)distance;
        _brushs.clear();
        for (int i = 0; i < d; ++i)
        {
            Sprite* sprite = Sprite::create("Images/fire.png");
            sprite->setColor(Color3B::RED);
            sprite->setOpacity(20);
            _brushs.pushBack(sprite);
        }
        for (int i = 0; i < d; i++)
        {
            float difx  = end.x - start.x;
            float dify  = end.y - start.y;
            float delta = (float)i / distance;
            _brushs.at(i)->setPosition(Vec2(start.x + (difx * delta), start.y + (dify * delta)));
            _brushs.at(i)->setRotation(rand() % 360);
            float r = (float)(rand() % 50 / 50.f) + 0.25f;
            _brushs.at(i)->setScale(r);
            /*_brush->setColor(Color3B(AXRANDOM_0_1() * 127 + 128, 255, 255));*/
            // Use AXRANDOM_0_1() will cause error when loading libtests.so on android, I don't know why.
            _brushs.at(i)->setColor(Color3B(rand() % 127 + 128, 255, 255));
            // Call visit to draw the brush, don't call draw..
            _brushs.at(i)->visit();
        }
    }

    // finish drawing and return context back to the screen
    _target->end();
}

/**
 * Implementation of RenderTextureIssue937
 */

RenderTextureIssue937::RenderTextureIssue937()
{
    /*
     *     1    2
     * A: A1   A2
     *
     * B: B1   B2
     *
     *  A1: premulti sprite
     *  A2: premulti render
     *
     *  B1: non-premulti sprite
     *  B2: non-premulti render
     */
    auto background = LayerColor::create(Color4B(200, 200, 200, 255));
    addChild(background);

    auto s            = Director::getInstance()->getWinSize();
    auto spr_premulti = Sprite::create("Images/fire.png");
    spr_premulti->setPosition(Vec2(s.width / 2 - 16, s.height / 2 + 16));

    auto spr_nonpremulti = Sprite::create("Images/fire.png");
    spr_nonpremulti->setPosition(Vec2(s.width / 2 - 16, s.height / 2 - 16));

    /* A2 & B2 setup */
    auto rend = RenderTexture::create(32, 64, backend::PixelFormat::RGBA8);

    if (nullptr == rend)
    {
        return;
    }

    auto spr_size = spr_premulti->getContentSize();
    rend->setKeepMatrix(true);
    Size pixelSize = Director::getInstance()->getWinSizeInPixels();
    rend->setVirtualViewport(Vec2(s.width / 2 - 32, s.height / 2 - 32), Rect(0, 0, s.width, s.height),
                             Rect(0, 0, pixelSize.width, pixelSize.height));

    // It's possible to modify the RenderTexture blending function by
    //        [[rend sprite] setBlendFunc:(BlendFunc) {BlendFactor::ONE, BlendFactor::ONE_MINUS_SRC_ALPHA}];
    rend->begin();
    spr_premulti->visit();
    spr_nonpremulti->visit();
    rend->end();

    rend->setPosition(Vec2(s.width / 2 + 16, s.height / 2));

    addChild(spr_nonpremulti);
    addChild(spr_premulti);
    addChild(rend);
}

std::string RenderTextureIssue937::title() const
{
    return "Testing issue #937";
}

std::string RenderTextureIssue937::subtitle() const
{
    return "All images should be equal...";
}

/**
 * Implementation of RenderTextureZbuffer
 */

RenderTextureZbuffer::RenderTextureZbuffer()
{
    auto listener            = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = AX_CALLBACK_2(RenderTextureZbuffer::onTouchesBegan, this);
    listener->onTouchesMoved = AX_CALLBACK_2(RenderTextureZbuffer::onTouchesMoved, this);
    listener->onTouchesEnded = AX_CALLBACK_2(RenderTextureZbuffer::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto size  = Director::getInstance()->getWinSize();
    auto label = Label::createWithTTF("vertexZ = 50", "fonts/Marker Felt.ttf", 64);
    label->setPosition(Vec2(size.width / 2, size.height * 0.25f));
    this->addChild(label);

    auto label2 = Label::createWithTTF("vertexZ = 0", "fonts/Marker Felt.ttf", 64);
    label2->setPosition(Vec2(size.width / 2, size.height * 0.5f));
    this->addChild(label2);

    auto label3 = Label::createWithTTF("vertexZ = -50", "fonts/Marker Felt.ttf", 64);
    label3->setPosition(Vec2(size.width / 2, size.height * 0.75f));
    this->addChild(label3);

    label->setPositionZ(50);
    label2->setPositionZ(0);
    label3->setPositionZ(-50);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/bugs/circle.plist");
    mgr = SpriteBatchNode::create("Images/bugs/circle.png", 9);
    this->addChild(mgr);
    sp1 = Sprite::createWithSpriteFrameName("circle.png");
    sp2 = Sprite::createWithSpriteFrameName("circle.png");
    sp3 = Sprite::createWithSpriteFrameName("circle.png");
    sp4 = Sprite::createWithSpriteFrameName("circle.png");
    sp5 = Sprite::createWithSpriteFrameName("circle.png");
    sp6 = Sprite::createWithSpriteFrameName("circle.png");
    sp7 = Sprite::createWithSpriteFrameName("circle.png");
    sp8 = Sprite::createWithSpriteFrameName("circle.png");
    sp9 = Sprite::createWithSpriteFrameName("circle.png");

    mgr->addChild(sp1, 9);
    mgr->addChild(sp2, 8);
    mgr->addChild(sp3, 7);
    mgr->addChild(sp4, 6);
    mgr->addChild(sp5, 5);
    mgr->addChild(sp6, 4);
    mgr->addChild(sp7, 3);
    mgr->addChild(sp8, 2);
    mgr->addChild(sp9, 1);

    sp1->setPositionZ(400);
    sp2->setPositionZ(300);
    sp3->setPositionZ(200);
    sp4->setPositionZ(100);
    sp5->setPositionZ(0);
    sp6->setPositionZ(-100);
    sp7->setPositionZ(-200);
    sp8->setPositionZ(-300);
    sp9->setPositionZ(-400);

    sp9->setScale(2);
    sp9->setColor(Color3B::YELLOW);
}

std::string RenderTextureZbuffer::title() const
{
    return "Testing Z Buffer in Render Texture";
}

std::string RenderTextureZbuffer::subtitle() const
{
    return "Touch screen. It should be green";
}

void RenderTextureZbuffer::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{

    for (auto& item : touches)
    {
        auto touch    = static_cast<Touch*>(item);
        auto location = touch->getLocation();

        sp1->setPosition(location);
        sp2->setPosition(location);
        sp3->setPosition(location);
        sp4->setPosition(location);
        sp5->setPosition(location);
        sp6->setPosition(location);
        sp7->setPosition(location);
        sp8->setPosition(location);
        sp9->setPosition(location);
    }
}

void RenderTextureZbuffer::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    for (auto& item : touches)
    {
        auto touch    = static_cast<Touch*>(item);
        auto location = touch->getLocation();

        sp1->setPosition(location);
        sp2->setPosition(location);
        sp3->setPosition(location);
        sp4->setPosition(location);
        sp5->setPosition(location);
        sp6->setPosition(location);
        sp7->setPosition(location);
        sp8->setPosition(location);
        sp9->setPosition(location);
    }
}

void RenderTextureZbuffer::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    this->renderScreenShot();
}

void RenderTextureZbuffer::renderScreenShot()
{
    auto texture = RenderTexture::create(512, 512);
    if (nullptr == texture)
    {
        return;
    }

    texture->begin();

    this->visit();

    texture->end();

    auto sprite = Sprite::createWithTexture(texture->getSprite()->getTexture());

    sprite->setPosition(Vec2(256, 256));
    sprite->setOpacity(182);
    sprite->setFlippedY(1);
    this->addChild(sprite, 999999);
    sprite->setColor(Color3B::GREEN);

    sprite->runAction(Sequence::create(FadeTo::create(2, 0), Hide::create(), nullptr));
}

RenderTexturePartTest::RenderTexturePartTest()
{
    auto sprite1     = Sprite::create("Images/grossini.png");
    auto sprite11    = Sprite::create("Images/grossini.png");
    auto sprite2     = Sprite::create("Images/grossinis_sister1.png");
    auto sprite22    = Sprite::create("Images/grossinis_sister1.png");
    Size size        = Director::getInstance()->getWinSize();
    Size sprite1Size = sprite1->getContentSize();
    sprite1->setPosition((size.width - sprite1Size.width) / 2 - 20, (size.height - sprite1Size.height) / 2 - 20);
    sprite11->setPosition(size.width / 2 + 20, (size.height - sprite1Size.height) / 2 - 20);

    sprite2->setPosition((size.width - sprite1Size.width) / 2 - 20, size.height / 2 + 20);
    sprite22->setPosition(size.width / 2 + 20, size.height / 2 + 20);

    addChild(sprite1);
    addChild(sprite11);
    addChild(sprite2);
    addChild(sprite22);

    _rend = RenderTexture::create(200, 200, backend::PixelFormat::RGBA8);
    _rend->retain();
    _rend->setKeepMatrix(true);
    Size pixelSize = Director::getInstance()->getWinSizeInPixels();
    _rend->setVirtualViewport(Vec2(size.width / 2 - 150, size.height / 2 - 150), Rect(0, 0, size.width, size.height),
                              Rect(0, 0, pixelSize.width, pixelSize.height));

    _rend->beginWithClear(1, 0, 0, 1);
    sprite1->visit();
    sprite11->visit();
    sprite2->visit();
    sprite22->visit();
    _rend->end();

    _spriteDraw                  = Sprite::createWithTexture(_rend->getSprite()->getTexture());
    FiniteTimeAction* baseAction = MoveBy::create(1, Vec2(size.width, 0.0f));
    _spriteDraw->setPosition(0, size.height / 2);
    _spriteDraw->setScaleY(-1);
    _spriteDraw->runAction(RepeatForever::create(Sequence::create(baseAction, baseAction->reverse(), nullptr)));
    addChild(_spriteDraw);
}

RenderTexturePartTest::~RenderTexturePartTest()
{
    AX_SAFE_RELEASE(_rend);
}

std::string RenderTexturePartTest::title() const
{
    return "Render Texture Part Test";
}

std::string RenderTexturePartTest::subtitle() const
{
    return "Only Grabbing a sub region of fullscreen";
}

// RenderTextureTestDepthStencil

RenderTextureTestDepthStencil::RenderTextureTestDepthStencil()
{
    auto s = _director->getWinSize();

    _renderer = _director->getRenderer();

    /// store old depth stencil desc
    _dsDesc = _renderer->getDepthStencilDesc();

    _spriteDS = Sprite::create("Images/fire.png");
    _spriteDS->retain();
    _spriteDS->setPosition(Vec2(s.width * 0.25f, 0.0f));
    _spriteDS->setScale(10);

    _spriteDraw = Sprite::create("Images/fire.png");
    _spriteDraw->retain();
    _spriteDraw->setPosition(Vec2(s.width * 0.25f, 0.0f));
    _spriteDraw->setScale(10);
    //! move sprite half width and height, and draw only where not marked
    _spriteDraw->setPosition(_spriteDraw->getPosition() +
                             Vec2(_spriteDraw->getContentSize().width * _spriteDraw->getScale() * 0.5f,
                                  _spriteDraw->getContentSize().height * _spriteDraw->getScale() * 0.5f));

    _rtx = RenderTexture::create(s.width, s.height, backend::PixelFormat::RGBA4, PixelFormat::D24S8);

    _rtx->setPosition(Vec2(s.width * 0.5f, s.height * 0.5f));

    this->addChild(_rtx);
}

RenderTextureTestDepthStencil::~RenderTextureTestDepthStencil()
{
    AX_SAFE_RELEASE(_spriteDraw);
    AX_SAFE_RELEASE(_spriteDS);

    // restore depth stencil desc
    _renderer->setDepthStencilDesc(_dsDesc);
    _renderer->setStencilTest(bitmask::any(_dsDesc.flags, DepthStencilFlags::STENCIL_TEST));
}

void RenderTextureTestDepthStencil::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
    _rtx->beginWithClear(0, 0, 0, 0, 0, 0);

//    _renderCmds[0].init(_globalZOrder);
//    _renderCmds[0].func = AX_CALLBACK_0(RenderTextureTestDepthStencil::onBeforeClear, this);
    renderer->addCallbackCommand(AX_CALLBACK_0(RenderTextureTestDepthStencil::onBeforeClear, this), _globalZOrder);

//    _renderCmds[1].init(_globalZOrder);
//    _renderCmds[1].func = AX_CALLBACK_0(RenderTextureTestDepthStencil::onBeforeStencil, this);
    renderer->addCallbackCommand(AX_CALLBACK_0(RenderTextureTestDepthStencil::onBeforeStencil, this), _globalZOrder);

    _spriteDS->visit();

//    _renderCmds[2].init(_globalZOrder);
//    _renderCmds[2].func = AX_CALLBACK_0(RenderTextureTestDepthStencil::onBeforeDraw, this);
    renderer->addCallbackCommand(AX_CALLBACK_0(RenderTextureTestDepthStencil::onBeforeDraw, this), _globalZOrder);

    _spriteDraw->visit();

//    _renderCmds[3].init(_globalZOrder);
//    _renderCmds[3].func = AX_CALLBACK_0(RenderTextureTestDepthStencil::onAfterDraw, this);
    renderer->addCallbackCommand(AX_CALLBACK_0(RenderTextureTestDepthStencil::onAfterDraw, this), _globalZOrder);

    /// !!!end will set current render target to default renderTarget
    /// !!!all render target share one depthStencilDesc, TODO: optimize me?
    _rtx->end();
}

void RenderTextureTestDepthStencil::onBeforeClear()
{
    _renderer->setStencilWriteMask(0xFF);
}

void RenderTextureTestDepthStencil::onBeforeStencil()
{
    //! mark sprite quad into stencil buffer
    _renderer->setStencilTest(true);
    _renderer->setStencilCompareFunction(backend::CompareFunction::NEVER, 1, 0xFF);
    _renderer->setStencilOperation(backend::StencilOperation::REPLACE, backend::StencilOperation::REPLACE,
                                   backend::StencilOperation::REPLACE);
}

void RenderTextureTestDepthStencil::onBeforeDraw()
{
    _renderer->setStencilCompareFunction(backend::CompareFunction::NOT_EQUAL, 1, 0xFF);
}

void RenderTextureTestDepthStencil::onAfterDraw()
{
    _renderer->setStencilTest(false);
}

std::string RenderTextureTestDepthStencil::title() const
{
    return "Testing depthStencil attachment";
}

std::string RenderTextureTestDepthStencil::subtitle() const
{
    return "Circle should be missing 1/4 of its region";
}

// RenderTextureTest
RenderTextureTargetNode::RenderTextureTargetNode()
{
    /*
     *     1    2
     * A: A1   A2
     *
     * B: B1   B2
     *
     *  A1: premulti sprite
     *  A2: premulti render
     *
     *  B1: non-premulti sprite
     *  B2: non-premulti render
     */
    auto background = LayerColor::create(Color4B(40, 40, 40, 255));
    addChild(background);

    // sprite 1
    sprite1 = Sprite::create("Images/fire.png");

    // sprite 2
    sprite2 = Sprite::create("Images/fire_rgba8888.pvr");

    auto s = Director::getInstance()->getWinSize();

    /* Create the render texture */
    auto renderTexture  = RenderTexture::create(s.width, s.height, backend::PixelFormat::RGBA4);
    this->renderTexture = renderTexture;

    renderTexture->setPosition(Vec2(s.width / 2, s.height / 2));
    // renderTexture->setScale(2.0f);

    /* add the sprites to the render texture */
    renderTexture->addChild(sprite1);
    renderTexture->addChild(sprite2);
    renderTexture->setClearColor(Color4F(0, 0, 0, 0));
    renderTexture->setClearFlags(ClearFlag::COLOR);

    /* add the render texture to the scene */
    addChild(renderTexture);

    renderTexture->setAutoDraw(true);

    scheduleUpdate();

    // Toggle clear on / off
    auto item = MenuItemFont::create("Clear On/Off", AX_CALLBACK_1(RenderTextureTargetNode::touched, this));
    auto menu = Menu::create(item, nullptr);
    addChild(menu);

    menu->setPosition(Vec2(s.width / 2, s.height / 2));
}

void RenderTextureTargetNode::touched(Object* sender)
{
    // TODO: minggo
    if (renderTexture->getClearFlags() == ClearFlag::NONE)
    {
        renderTexture->setClearFlags(ClearFlag::COLOR);
    }
    else
    {
        renderTexture->setClearFlags(ClearFlag::NONE);
        renderTexture->setClearColor(Color4F(AXRANDOM_0_1(), AXRANDOM_0_1(), AXRANDOM_0_1(), 1));
    }
}

void RenderTextureTargetNode::update(float dt)
{
    static float time = 0;
    float r           = 80;
    sprite1->setPosition(Vec2(cosf(time * 2) * r, sinf(time * 2) * r));
    sprite2->setPosition(Vec2(sinf(time * 2) * r, cosf(time * 2) * r));

    time += dt;
}

std::string RenderTextureTargetNode::title() const
{
    return "Testing Render Target Node";
}

std::string RenderTextureTargetNode::subtitle() const
{
    return "Sprites should be equal and move with each frame";
}

// SpriteRenderTextureBug

SpriteRenderTextureBug::SimpleSprite::SimpleSprite() : _rt(nullptr) {}
SpriteRenderTextureBug::SimpleSprite::~SimpleSprite()
{
    AX_SAFE_RELEASE(_rt);
}

SpriteRenderTextureBug::SimpleSprite* SpriteRenderTextureBug::SimpleSprite::create(const char* filename,
                                                                                   const Rect& rect)
{
    auto sprite = new SimpleSprite();
    if (sprite->initWithFile(filename, rect))
    {
        sprite->autorelease();
    }
    else
    {
        AX_SAFE_DELETE(sprite);
    }

    return sprite;
}

void SpriteRenderTextureBug::SimpleSprite::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
    if (_rt == nullptr)
    {
        auto s = Director::getInstance()->getWinSize();
        _rt    = RenderTexture::create(s.width, s.height, backend::PixelFormat::RGBA8);
        _rt->retain();
    }
    _rt->beginWithClear(0.0f, 0.0f, 0.0f, 1.0f);
    _rt->end();

    Sprite::draw(renderer, transform, flags);
}

SpriteRenderTextureBug::SpriteRenderTextureBug()
{
    auto listener            = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = AX_CALLBACK_2(SpriteRenderTextureBug::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto s = Director::getInstance()->getWinSize();
    addNewSpriteWithCoords(Vec2(s.width / 2, s.height / 2));
}

SpriteRenderTextureBug::SimpleSprite* SpriteRenderTextureBug::addNewSpriteWithCoords(const Vec2& p)
{
    int idx = AXRANDOM_0_1() * 1400 / 100;
    int x   = (idx % 5) * 85;
    int y   = (idx / 5) * 121;

    auto sprite = SpriteRenderTextureBug::SimpleSprite::create("Images/grossini_dance_atlas.png", Rect(x, y, 85, 121));
    addChild(sprite);

    sprite->setPosition(p);

    FiniteTimeAction* action = nullptr;
    float rd                 = AXRANDOM_0_1();

    if (rd < 0.20)
        action = ScaleBy::create(3, 2);
    else if (rd < 0.40)
        action = RotateBy::create(3, 360);
    else if (rd < 0.60)
        action = Blink::create(1, 3);
    else if (rd < 0.8)
        action = TintBy::create(2, 0, -255, -255);
    else
        action = FadeOut::create(2);

    auto action_back = action->reverse();
    auto seq         = Sequence::create(action, action_back, nullptr);

    sprite->runAction(RepeatForever::create(seq));

    // return sprite;
    return nullptr;
}

void SpriteRenderTextureBug::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    for (auto& touch : touches)
    {
        auto location = touch->getLocation();
        addNewSpriteWithCoords(location);
    }
}

std::string SpriteRenderTextureBug::title() const
{
    return "SpriteRenderTextureBug";
}

std::string SpriteRenderTextureBug::subtitle() const
{
    return "Touch the screen. Sprite should appear on under the touch";
}

//
// Issue16113Test
//
Issue16113Test::Issue16113Test()
{
    auto s = Director::getInstance()->getWinSize();

    // Save Image menu
    MenuItemFont::setFontSize(16);
    auto item1 = MenuItemFont::create("Save Image", [&](Object* ref) {
        auto winSize = Director::getInstance()->getVisibleSize();
        auto text    = Label::createWithTTF("hello world", "fonts/Marker Felt.ttf", 40);
        text->setTextColor(Color4B::RED);
        auto target = RenderTexture::create(winSize.width, winSize.height, backend::PixelFormat::RGBA8);
        target->beginWithClear(0, 0, 0, 0);
        text->setPosition(winSize.width / 2, winSize.height / 2);
        text->Node::visit();
        target->end();
        auto callback = [this](RenderTexture* rt, std::string_view path) { rt->release(); };
        target->retain();
        target->saveToFile("issue16113.png", Image::Format::PNG, true, callback);
    });
    auto menu  = Menu::create(item1, nullptr);
    this->addChild(menu);
    menu->setPosition(s.width / 2, s.height / 2);
}

std::string Issue16113Test::title() const
{
    return "Github Issue 16113";
}

std::string Issue16113Test::subtitle() const
{
    return "aaa.png file without white border on iOS";
}
