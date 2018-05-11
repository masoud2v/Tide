/*********************************************************************/
/* Copyright (c) 2017-2018, EPFL/Blue Brain Project                  */
/*                          Raphael Dumusc <raphael.dumusc@epfl.ch>  */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of Ecole polytechnique federale de Lausanne.          */
/*********************************************************************/

#ifndef PIXELSTREAMASSEMBLER_H
#define PIXELSTREAMASSEMBLER_H

#include "types.h"

#include "PixelStreamChannelAssembler.h"
#include "PixelStreamProcessor.h"

#include <deflect/server/Frame.h>

#include <map>

/**
 * Assemble small frame tiles (e.g. 64x64) into 512x512 ones.
 *
 * This class operates on all the channels of the frame. The tile indices are
 * global for the frame, i.e. individual channels are mapped to contiguous,
 * non-overlapping sets of indices by computeVisibleSet().
 */
class PixelStreamAssembler : public PixelStreamProcessor
{
public:
    /**
     * Create an assembler for the frame.
     * @param frame to assemble with tiles sorted left-right + top-bottom.
     * @throw std::runtime_error if the frame cannot be assembled.
     */
    PixelStreamAssembler(deflect::server::FramePtr frame);

    /** @copydoc PixelStreamProcessor::getTileImage */
    ImagePtr getTileImage(uint tileIndex,
                          deflect::server::TileDecoder& decoder) final;

    /** @copydoc PixelStreamProcessor::getTileRect */
    QRect getTileRect(uint tileIndex) const final;

    /** @copydoc PixelStreamProcessor::computeVisibleSet */
    Indices computeVisibleSet(const QRectF& visibleArea,
                              uint channel) const final;

private:
    struct Channel
    {
        mutable PixelStreamChannelAssembler assembler;
        const size_t offset = 0;
        const size_t tilesCount = 0;

        Channel(deflect::server::FramePtr frame, const uint channel,
                const size_t offset_)
            : assembler(frame, channel)
            , offset{offset_}
            , tilesCount{assembler.getTilesCount()}
        {
        }
    };
    std::vector<Channel> _channels;

    bool _parseChannels(deflect::server::FramePtr frame);
    const Channel& _getChannel(uint tileIndex) const;
};

#endif
