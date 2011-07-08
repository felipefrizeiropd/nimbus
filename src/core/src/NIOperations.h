//
// Copyright 2011 Jeff Verkoeyen
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#ifdef NIMBUS_STATIC_LIBRARY
#import "NimbusCore/NIBlocks.h"
#else
#import "NIBlocks.h"
#endif

/**
 * For common NSOperation implementations.
 *
 * @ingroup NimbusCore
 * @defgroup Operations Operations
 *
 * Some tasks take time to complete and are best done asynchronously. This collection of
 * operations is meant to provide a minimum set of functionality for common tasks to be
 * performed asynchronously.
 */

/**
 * For crafting operations.
 *
 * @ingroup Operations
 * @defgroup Crafting-Operations Crafting Operations
 */

@protocol NIOperationDelegate;

/**
 * A base implementation of an NSOperation that supports traditional delegation and blocks.
 *
 *      @ingroup Crafting-Operations
 *
 * A subclass should call the operationDid* methods to notify the delegate on the main thread
 * of changes in the operation's state. Calling these methods will notify the delegate and the
 * blocks if provided.
 */
@interface NIOperation : NSOperation {
@private
  id<NIOperationDelegate> _delegate;

  NSError*                _lastError;

#if NS_BLOCKS_AVAILABLE
  NIBasicBlock _didStartBlock;
  NIBasicBlock _didFinishBlock;
  NIErrorBlock _didFailWithErrorBlock;
#endif // #if NS_BLOCKS_AVAILABLE
}

/**
 * The delegate through which changes are notified for this operation.
 */
@property (nonatomic, readwrite, assign) id<NIOperationDelegate> delegate;

/**
 * The error last passed to the didFailWithError notification.
 */
@property (nonatomic, readonly, retain) NSError* lastError;

#if NS_BLOCKS_AVAILABLE

/**
 * The operation has started executing.
 */
@property (nonatomic, readwrite, copy) NIBasicBlock didStartBlock;

/**
 * The operation has completed successfully.
 *
 * This will not be called if the operation fails.
 */
@property (nonatomic, readwrite, copy) NIBasicBlock didFinishBlock;

/**
 * The operation failed in some way and has completed.
 *
 * didFinishBlock will not be executed.
 */
@property (nonatomic, readwrite, copy) NIErrorBlock didFailWithErrorBlock;

#endif // #if NS_BLOCKS_AVAILABLE


/**
 * @name Subclassing
 * @{
 *
 * The following methods are provided to aid in subclassing and are not meant to be
 * used externally.
 */
#pragma mark Subclassing

/**
 * On the main thread, notify the delegate that the operation has begun.
 */
- (void)operationDidStart;

/**
 * On the main thread, notify the delegate that the operation has finished.
 */
- (void)operationDidFinish;

/**
 * On the main thread, notify the delegate that the operation has failed.
 */
- (void)operationDidFailWithError:(NSError *)error;

/**@}*/

@end


/**
 * An operation that reads a file from disk.
 *
 *      @ingroup Operations
 *
 * Provides asynchronous file reading support when added to an NSOperationQueue.
 *
 * It is recommended to add this operation to a serial NSOperationQueue to avoid overlapping
 * disk read attempts. This will noticeably improve performance when loading many files
 * from disk at once.
 */
@interface NIReadFileFromDiskOperation : NIOperation {
@private
  // [in]
  NSString* _pathToFile;

  // [out]
  NSData*   _data;
}


/**
 * @name Creating an operation
 * @{
 */
#pragma mark Creating an operation

/**
 * Designated initializer.
 */
- (id)initWithPathToFile:(NSString *)pathToFile;

/**@}*/


/**
 * @name Configuring the Operation
 * @{
 */
#pragma mark Configuring the operation

/**
 * The path to the file that should be read from disk.
 */
@property (nonatomic, readwrite, copy) NSString* pathToFile;

/**@}*/


/**
 * @name Operation Results
 * @{
 */
#pragma mark Operation Results

/**
 * The data that was read from disk.
 *
 * Will be nil if the data couldn't be read.
 *
 *      @sa NIOperation::lastError
 */
@property (nonatomic, readonly, retain) NSData* data;

/**@}*/

@end


/**
 * The delegate protocol for an NSOperation.
 *
 *      @ingroup Operations
 */
@protocol NIOperationDelegate <NSObject>
@optional

/**
 * The operation has started executing.
 */
- (void)operationDidStart:(NSOperation *)operation;

/**
 * The operation has completed successfully.
 *
 * This will not be called if the operation fails.
 */
- (void)operationDidFinish:(NSOperation *)operation;

/**
 * The operation failed in some way and has completed.
 *
 * operationDidFinish will not be called.
 */
- (void)operationDidFail:(NSOperation *)operation withError:(NSError *)error;

@end
