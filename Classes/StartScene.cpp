//
//  StartScene.cpp
//  LLK
//
//  Created by 白菜哥 on 16/9/16.
//
//

#include "StartScene.hpp"
#include "GameConfig.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"
#include "GameScene.hpp"

using namespace CocosDenshion;

Scene *StartScene::createScene(){
    
    auto scene = Scene::create();
    
    auto startScene = StartScene::create();
    
    scene->addChild(startScene);
    
    return scene;
}

bool StartScene::init(){

    if (!Layer::init()) {
        
        return false;
    }
    
    auto loading = Sprite::create(s_loading_logo);
    loading->setPosition(screenSize.width/2,screenSize.height/2);
    addChild(loading);
    
    loading->setOpacity(50);
    
    auto fadeIn = FadeIn::create(1);
    loading->runAction(RepeatForever::create(Sequence::create(fadeIn, fadeIn->reverse() ,nullptr)));
    
    {
        utils::gettime();
        // 帧缓存
        auto spriteFrameCache = SpriteFrameCache::getInstance();
        // 纹理缓存
        auto textureCache = Director::getInstance()->getTextureCache();
        textureCache->addImage(s_backgound);
        textureCache->addImage(s_backgound_cloud1);
        textureCache->addImage(s_backgound_cloud2);
        textureCache->addImage(s_backgound_sea1);
        textureCache->addImage(s_backgound_sea2);
        textureCache->addImage(s_beach_adornment);
        textureCache->addImage(s_island);
        textureCache->addImage(s_button_play);
        textureCache->addImage(s_button_play_s);
        textureCache->addImage(s_time_bars);
        textureCache->addImage(s_time_slot);
        textureCache->addImage(s_game_dialog);
        textureCache->addImage(s_game_leisure);
        
        textureCache->addImage(s_game_bgmOn);
        textureCache->addImage(s_game_bgmOff);
        textureCache->addImage(s_game_help);
        
        spriteFrameCache->addSpriteFramesWithFile(s_boat_plist);
        spriteFrameCache->addSpriteFramesWithFile(s_seagull_plist);
        spriteFrameCache->addSpriteFramesWithFile(s_icon_plist);
        
        // 加载声音资源
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_Back2new);
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_Back3new);
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_class);
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_mainmenu);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_2);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_3);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_4);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_12);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_14);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_15);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_16);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_19);
        
        scheduleOnce(SEL_SCHEDULE(&::StartScene::initUI), 2);
    }
    
    return true;
}

#pragma mark 初始化界面

void StartScene::initUI(){

    this->removeAllChildren();
    
    
    
    
    auto textureCache = Director::getInstance()->getTextureCache();
    
    auto spriteFrameCache = SpriteFrameCache::getInstance();
    
    {
        auto background = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound));
        background->setPosition(screenSize.width/2,screenSize.height/2);
        addChild(background);
    }
    
    {
        auto cloud_1 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud1));
        cloud_1->setAnchorPoint(Vec2(0, 0));
        cloud_1->setPosition(-screenSize.width/2+40,screenSize.height - cloud_1->getContentSize().height+36);
        addChild(cloud_1);
        
        auto cloudShadow = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud1));
        cloudShadow->setAnchorPoint(Vec2(0,0));
        
        cloudShadow->setFlippedY(true);
        cloudShadow->setOpacity(40);
        cloudShadow->setPosition(-screenSize.width/2+40,screenSize.height-cloudShadow->getContentSize().height*2+36);
        addChild(cloudShadow);
    
        auto cloud_2 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud2));
        cloud_2->setAnchorPoint(Vec2(0,0));
        cloud_2->setPosition(cloud_1->getContentSize().width/2,screenSize.height-cloud_2->getContentSize().height+36);
        addChild(cloud_2);
        
        auto island = Sprite::createWithTexture(textureCache->getTextureForKey(s_island));
        island->setAnchorPoint(Vec2(1, 0));
        island->setPosition(screenSize.width-40 , screenSize.height-island->getContentSize().height-47*2);
        addChild(island);
        
        auto islandShadow = Sprite::createWithTexture(textureCache->getTextureForKey(s_island));
        islandShadow->setAnchorPoint(Vec2(1, 0));
        islandShadow->setFlippedY(true);
        islandShadow->setOpacity(40);
        islandShadow->setPosition(screenSize.width-40,screenSize.height-islandShadow->getContentSize().height-78*2);
        addChild(islandShadow);
        
        auto frame = spriteFrameCache->getSpriteFrameByName("sailing_boat1.png");
        auto boat_1 = Sprite::createWithSpriteFrame(frame);
        boat_1->setPosition(screenSize.width-boat_1->getContentSize().width-50*2,230*2);
        
        
        auto animation = Animation::create();
        
        for (int i = 1; i<4; i++) {
            char bfileName[64] ={0};
            sprintf(bfileName, "sailing_boat%d.png",i);
            animation->addSpriteFrame(spriteFrameCache->getSpriteFrameByName(bfileName));
        }
        animation->setDelayPerUnit(0.5);
        animation->setRestoreOriginalFrame(true);
        addChild(boat_1);
        
        auto animate = Animate::create(animation);
        boat_1->runAction(RepeatForever::create(animate));
        
        auto moveBy = MoveBy::create(10, Vec2(-240, 0));
        auto flipX = FlipX::create(true);
        auto action = Sequence::create(moveBy,flipX,moveBy->reverse(),flipX->reverse(), NULL);
        boat_1->runAction(RepeatForever::create(action));
        
        auto boat_2 = Sprite::createWithSpriteFrame(frame);
        boat_2->setPosition(100,400);
        boat_2->setFlippedX(true);
        addChild(boat_2);
        boat_2->runAction(animate->clone());
        
        auto moveBy_2 = MoveBy::create(12, Vec2(300, 0));
        auto action_2 = Sequence::create(moveBy_2,flipX->clone()->reverse(),moveBy_2->reverse(),flipX->clone(), NULL);
        boat_2->runAction(RepeatForever::create(action_2));
        
        auto beach = Sprite::createWithTexture(textureCache->getTextureForKey(s_beach_adornment));
        beach->setPosition(screenSize.width/2,beach->getContentSize().height/2);
        addChild(beach);
        
        auto wave_1 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_sea1));
        wave_1->setPosition(screenSize.width/2,140*2);
        wave_1->setOpacity(0);
        wave_1->setScale(1.2, 1);
        addChild(wave_1);
        
        auto wave_2 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_sea2));
        wave_2->setPosition(screenSize.width / 2 , 100 * 2);
        wave_2->setOpacity(0);
        wave_2->setScale(1.2, 1);
        addChild(wave_2);
        
        
        auto waveMoveBy = MoveBy::create(4.0, Vec2(0, -80));
        auto fadeIn = FadeIn::create(3.0);
        auto fadeOut = FadeOut::create(1.0);
        
        auto place_1 = Place::create(Vec2(screenSize.width/2,140*2));
        auto spawn_1 = Spawn::create(waveMoveBy,Sequence::create(fadeIn,fadeOut, NULL), NULL);
        auto spawn_2 = Spawn::create(waveMoveBy->clone()->reverse(),Sequence::create(fadeIn->clone(),fadeOut->clone(), NULL), NULL);
        auto place_2 = Place::create(Vec2(screenSize.width/2, 100*2));
        
        auto targetAction = TargetedAction::create(wave_2, Sequence::create(spawn_2,place_2, NULL));
        auto delay = DelayTime::create(2.0);
        wave_1->runAction(RepeatForever::create(Sequence::create(spawn_1,targetAction,delay,place_1, NULL)));
        
        auto birdFrame = spriteFrameCache->getSpriteFrameByName("seagull1.png");
        pBird = Sprite::createWithSpriteFrame(birdFrame);
        auto point = Vec2(CCRANDOM_0_1() * screenSize.width, 0);
        pBird->setPosition(point);
        addChild(pBird);
        
        auto birdAnimation = Animation::create();
        for (int i = 1; i<4; i++) {
            char bname[64] = {0};
            
            sprintf(bname, "seagull%d.png", i);
            
            birdAnimation->addSpriteFrame(spriteFrameCache->getSpriteFrameByName(bname));
        }
        birdAnimation->setDelayPerUnit(0.3);
        birdAnimation->setRestoreOriginalFrame(true);
        auto birdAnimate = Animate::create(birdAnimation);
        pBird->runAction(RepeatForever::create(birdAnimate));
        auto birdMoveBy = MoveBy::create(2, Vec2(CCRANDOM_0_1() * screenSize.width , point.y + screenSize.height + 100));
        pBird->runAction(birdMoveBy);
        
        scheduleUpdate();
        
        auto normal = Sprite::createWithTexture(textureCache->getTextureForKey(s_button_play));
        auto select = Sprite::createWithTexture(textureCache->getTextureForKey(s_button_play_s));
        auto start = MenuItemSprite::create(normal, select, CC_CALLBACK_0(StartScene::gameStart, this));
        start->setPosition(screenSize.width/2,screenSize.height/2);
        auto menu = Menu::create(start, NULL);
        menu->setPosition(Vec2::ZERO);
        addChild(menu);
    }
}

#pragma mark 刷新飞鸟
void StartScene::update(float dt){
    
    if (pBird->getPosition().y > screenSize.height + 20) {
        auto point = Vec2(CCRANDOM_0_1() * screenSize.width, 10);
        pBird->setPosition(point);
        pBird->runAction(MoveBy::create(floor(6 * CCRANDOM_0_1()), Vec2(CCRANDOM_0_1() * screenSize.width, point.y + screenSize.height+20)));
    }
}

#pragma mark 游戏开始
void StartScene::gameStart(){

    auto gameScene = GameScene::createScene();
    //Director::getInstance()->replaceScene(gameScene);
    Director::getInstance()->replaceScene(TransitionRotoZoom::create(1.2, gameScene));
}