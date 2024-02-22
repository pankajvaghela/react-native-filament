//
//  AppleFilamentView.h
//  Pods
//
//  Created by Marc Rousavy on 22.02.24.
//

#pragma once

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface AppleFilamentView : UIView

- (instancetype) init;

- (CAMetalLayer*) layer;

@end

NS_ASSUME_NONNULL_END
