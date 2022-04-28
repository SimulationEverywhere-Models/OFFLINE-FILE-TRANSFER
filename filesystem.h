/*******************************************************************
 *
 *  DESCRIPTION: FileSystem
 *
 *  AUTHOR: Weiwei Li
 *
 *  EMAIL: mailto://wli086@uottawa.ca
 *
 *  DATE: 08/10/2012
 *
 *******************************************************************/

#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include <queue>
#pragma data_seg( "share" )
static queue<int> tmpFile;
#pragma data_seg()
#pragma comment( linker,"/SECTION:share,RWS" )

#endif   //__FILESYSTEM_H
