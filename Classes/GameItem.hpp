//
//  GameItem.hpp
//  LLK
//
//  Created by 白菜哥 on 16/9/23.
//
//

#ifndef GameItem_hpp
#define GameItem_hpp

#include <stdio.h>

USING_NS_CC;

class GameItem : public Sprite {

public:
    
    ~GameItem();
    static GameItem *createItem(SpriteFrame *spriteFrame);
    void initBasic();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif /* GameItem_hpp */
