/*********************************************************************/
/* Copyright (c) 2015-2018, EPFL/Blue Brain Project                  */
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
/*    THIS  SOFTWARE  IS  PROVIDED  BY  THE  ECOLE  POLYTECHNIQUE    */
/*    FEDERALE DE LAUSANNE  ''AS IS''  AND ANY EXPRESS OR IMPLIED    */
/*    WARRANTIES, INCLUDING, BUT  NOT  LIMITED  TO,  THE  IMPLIED    */
/*    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR  A PARTICULAR    */
/*    PURPOSE  ARE  DISCLAIMED.  IN  NO  EVENT  SHALL  THE  ECOLE    */
/*    POLYTECHNIQUE  FEDERALE  DE  LAUSANNE  OR  CONTRIBUTORS  BE    */
/*    LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,    */
/*    EXEMPLARY,  OR  CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT NOT    */
/*    LIMITED TO,  PROCUREMENT  OF  SUBSTITUTE GOODS OR SERVICES;    */
/*    LOSS OF USE, DATA, OR  PROFITS;  OR  BUSINESS INTERRUPTION)    */
/*    HOWEVER CAUSED AND  ON ANY THEORY OF LIABILITY,  WHETHER IN    */
/*    CONTRACT, STRICT LIABILITY,  OR TORT  (INCLUDING NEGLIGENCE    */
/*    OR OTHERWISE) ARISING  IN ANY WAY  OUT OF  THE USE OF  THIS    */
/*    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.   */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of Ecole polytechnique federale de Lausanne.          */
/*********************************************************************/

#ifndef WALLWINDOW_H
#define WALLWINDOW_H

#include "types.h"

#include <deflect/qt/types.h>

#include <QQuickWindow>

class QQuickRenderControl;
class QQmlEngine;

/**
 * An OpenGL window in which the Qml scene is rendered.
 */
class WallWindow : public QQuickWindow
{
    Q_OBJECT

public:
    /**
     * Create a wall window.
     * @param config the wall configuration to setup this window wrt position,
     *               size, etc.
     * @param windowIndex the index of the window for this process.
     * @param provider the provider of data for the windows.
     */
    static WallWindowPtr create(const WallConfiguration& config,
                                uint windowIndex, DataProvider& provider);

    /**
     * Create all the windows for the wall process.
     * @param config the wall configuration to setup this window wrt position,
     *               size, etc.
     * @param provider the provider of data for the windows.
     */
    static std::vector<WallWindowPtr> createWindows(
        const WallConfiguration& config, DataProvider& provider);

    ~WallWindow();

    /** @return the index of the surface that the window belongs to. */
    size_t getSurfaceIndex() const;

    /**
     * Set a swap synchronizer.
     *
     * @param synchronizer to synchronize swapBuffers() (optional)
     */
    void setSwapSynchronizer(SwapSynchronizer* synchronizer);

    bool isInitialized() const;
    bool needRedraw() const;

    /**
     * Synchronize scene objects with render thread and trigger frame rendering.
     *
     * @param grab indicate that the frame should be grabbed after rendering.
     */
    void render(bool grab = false);

    /** Set new surface. */
    void setSurface(SurfacePtr surface);

    /** Set new screen lock. */
    void setScreenLock(ScreenLockPtr lock);

    /** Set new countdown status. */
    void setCountdownStatus(CountdownStatusPtr status);

    /** Set new touchpoint's markers. */
    void setMarkers(MarkersPtr markers);

    /** Set new render options. */
    void setRenderOptions(OptionsPtr options);

signals:
    /** Emitted after syncAndRender() has been called with grab set to true. */
    void imageGrabbed(QImage image, QPoint index);

private:
    WallWindow(const WallConfiguration& config, uint windowIndex,
               DataProvider& provider,
               std::unique_ptr<QQuickRenderControl> renderControl);

    void exposeEvent(QExposeEvent* exposeEvent) final;

    void _startQuickRenderer();
    void _setupScene(const WallConfiguration& config, uint windowIndex);

    DataProvider& _provider;

    size_t _surfaceIndex = 0;
    QPoint _globalIndex;

    std::unique_ptr<QQuickRenderControl> _renderControl;
    SwapSynchronizer* _synchronizer = nullptr;
    bool _grabImage = false;

    std::unique_ptr<deflect::qt::QuickRenderer> _quickRenderer;
    std::unique_ptr<QThread> _quickRendererThread;
    std::unique_ptr<QQmlEngine> _qmlEngine;
    std::unique_ptr<WallSurfaceRenderer> _surfaceRenderer;
    std::unique_ptr<TestPattern> _testPattern;
};

#endif
