//
//  GameItem.cpp
//  LLK
//
//  Created by 白菜哥 on 16/9/23.
//
//

#include "GameItem.hpp"


GameItem::~GameItem(){}

GameItem *GameItem::createItem(SpriteFrame *spriteFrame){

    GameItem *item = new GameItem();
    
    if (item && item->initWithSpriteFrame(spriteFrame)) {
        item->autorelease();
        item->initBasic();
        return item;
    }
    CC_SAFE_DELETE(item);
    return nullptr;
}

void GameItem::initBasic(){

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameItem::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameItem::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameItem::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameItem::onTouchCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
}

bool GameItem::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){


    this->setScale(1.2);
    return true;
}

void GameItem::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){

}

void GameItem::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event){

}

void GameItem::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){

    auto point = touch->getLocation();

}