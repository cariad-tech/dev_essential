/**
 * @file
 * ADTF System interface.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#ifndef _ADTF_SYSTEM_INTF_HEADER_
#define _ADTF_SYSTEM_INTF_HEADER_

namespace adtf {
namespace system {
namespace ant {

class IADTFSystem {
public:
    virtual const tChar* GetSystemUrl() = 0;
    virtual const tChar* GetSystemPropertiesUrl() = 0;
    virtual const tChar* GetGraphUrl() = 0;
    virtual const tChar* GetGraphPropertiesUrl() = 0;

    virtual const tChar* GetLastErrorString() = 0;
    virtual const tChar* GetSessionName() = 0;

    virtual tResult LoadGraphFromString(tChar* strGraph) = 0;
    virtual tResult LoadGraphFromUrl(tChar* strGraphFile) = 0;

    virtual tResult LoadGraphPropertiesFromString(tChar* strProperties) = 0;
    virtual tResult LoadGraphPropertiesFromUrl(tChar* strPropertiesFile) = 0;

    virtual const tChar* GetActiveFilterGraph() = 0;
    virtual tResult SetActiveFilterGraph(tChar* strConfig) = 0;

    virtual const tChar* GetActiveStreamingGraph() = 0;
    virtual tResult SetActiveStreamingGraph(tChar* strConfig) = 0;

    virtual tInt8 GetRunlevel() = 0;
    virtual tResult SetRunlevel(tInt8 nRunLevel) = 0;

    virtual tSize GetPlaybackMode() = 0;
    virtual tFloat GetPlaybackRate() = 0;
    virtual tResult SetPlaybackRate(tFloat nRate) = 0;
    virtual tTimeStamp GetStreamDuration() = 0;
    virtual tResult Pause() = 0;
    virtual tResult Resume() = 0;
    virtual tResult Seek(tTimeStamp nAbsTime) = 0;
    virtual tResult Seek(tTimeStamp nOffsetTime, tBool bForward) = 0;

    virtual tTimeStamp GetStreamTime() = 0;
    virtual tFloat GetDataRate() = 0;
    virtual tFloat GetSampleRate() = 0;

    virtual tResult RegisterListener(tUInt8 nFlags, tChar* strUrl) = 0;
    virtual tResult UnregisterListener(tChar* strUrl) = 0;

    virtual const tChar* GetRecorderIds() = 0;
    virtual tResult StartRecording(const tChar* strID, const tChar* strFileName) = 0;
    virtual tResult StopRecording(const tChar* strID, const tChar* strFileName) = 0;
    virtual tResult SplitRecording(const tChar* strID, const tChar* strFileName) = 0;

    virtual tResult Shutdown() = 0;
};

} // namespace ant

using ant::IADTFSystem;

} // namespace system
} // namespace adtf

#endif // _ADTF_SYSTEM_INTF_HEADER_
