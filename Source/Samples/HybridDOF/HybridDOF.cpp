/***************************************************************************
 # Copyright (c) 2015-22, NVIDIA CORPORATION. All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions
 # are met:
 #  * Redistributions of source code must retain the above copyright
 #    notice, this list of conditions and the following disclaimer.
 #  * Redistributions in binary form must reproduce the above copyright
 #    notice, this list of conditions and the following disclaimer in the
 #    documentation and/or other materials provided with the distribution.
 #  * Neither the name of NVIDIA CORPORATION nor the names of its
 #    contributors may be used to endorse or promote products derived
 #    from this software without specific prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY
 # EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 # PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 # CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 # EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 # PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 # PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 # OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 # OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **************************************************************************/
#include "HybridDOF.h"

#if FALCOR_D3D12_AVAILABLE
FALCOR_EXPORT_D3D12_AGILITY_SDK
#endif

uint32_t mSampleGuiWidth = 250;
uint32_t mSampleGuiHeight = 200;
uint32_t mSampleGuiPositionX = 20;
uint32_t mSampleGuiPositionY = 40;

void HybridDOF::onGuiRender(Gui* pGui)
{
    Gui::Window w(pGui, "Falcor", { 250, 200 });
    gpFramework->renderGlobalUI(pGui);
    w.text("Hello from HybridDOF");
    if (w.button("Click Here"))
    {
        msgBox("Now why would you do that?");
    }
}

void HybridDOF::onLoad(RenderContext* pRenderContext)
{
}

void HybridDOF::onFrameRender(RenderContext* pRenderContext, const Fbo::SharedPtr& pTargetFbo)
{
    const float4 clearColor(0.38f, 0.52f, 0.10f, 1);
    pRenderContext->clearFbo(pTargetFbo.get(), clearColor, 1.0f, 0, FboAttachmentType::All);
}

void HybridDOF::onShutdown()
{
}

bool HybridDOF::onKeyEvent(const KeyboardEvent& keyEvent)
{
    return false;
}

bool HybridDOF::onMouseEvent(const MouseEvent& mouseEvent)
{
    return false;
    //return mpScene && mpScene->onMouseEvent(mouseEvent);
}

void HybridDOF::onHotReload(HotReloadFlags reloaded)
{
}

void HybridDOF::onResizeSwapChain(uint32_t width, uint32_t height)
{
    // don't resize
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    HybridDOF::UniquePtr pRenderer = std::make_unique<HybridDOF>();
    SampleConfig config;
    config.windowDesc.title = "HybridDoF";
    config.windowDesc.resizableWindow = false;
    Sample::run(config, pRenderer);
    return 0;
}
