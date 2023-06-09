#include <chrono>
#include <cmath>

#include <openfx/ofxImageEffect.h>
#include <openfx/ofxProperty.h>
#include <openfx/ofxParam.h>
#include <openfx/ofxMemory.h>
#include <openfx/ofxMultiThread.h>

#include "Host.h"
#include "PluginCache.h"
#include "../logging.h"

#include "suites/ImageEffect.h"
#include "suites/Property.h"
#include "suites/Param.h"
#include "suites/Memory.h"
#include "suites/Multithread.h"

std::tuple<uint8_t, uint8_t, uint8_t> yuv2rgb_r(uint8_t y, uint8_t u, uint8_t v)
{
	float yn = (float)y / 255.0f;
	float un = (float)u / 255.0f;
	float vn = (float)v / 255.0f;

	float r = yn + 1.28033f * vn;
	float g = yn - 0.21482f * un - 0.38059f * vn;
	float b = yn + 2.12798f * un;

	r = std::min(1.0f, std::max(0.0f, r));
	g = std::min(1.0f, std::max(0.0f, g));
	b = std::min(1.0f, std::max(0.0f, b));

	return std::make_tuple((uint8_t)(r * 255.f), (uint8_t)(g * 255.f),
			       (uint8_t)(b * 255.f));
}

obs_source_frame *create_rgba_from_yuv422_frame(obs_source_frame *yuv_frame)
{
	obs_source_frame *rgb_frame = obs_source_frame_create(
		VIDEO_FORMAT_RGBA, yuv_frame->width, yuv_frame->height);

	for (size_t y = 0; y < yuv_frame->height; y++) {
		for (size_t x = 0; x < yuv_frame->width; x++) {
			size_t idx = y * rgb_frame->linesize[0] + 4 * x;
			rgb_frame->data[0][idx + 0] = yuv_frame->data[0][y * yuv_frame->linesize[0] + x];
			rgb_frame->data[0][idx + 1] = yuv_frame->data[0][y * yuv_frame->linesize[0] + x];
			rgb_frame->data[0][idx + 2] = yuv_frame->data[0][y * yuv_frame->linesize[0] + x];
			rgb_frame->data[0][idx + 3] = 255;

			rgb_frame->data[0][idx + (y / 360)] = 255;
		}
	}

	rgb_frame->timestamp = yuv_frame->timestamp;

	return rgb_frame;
}

ofx::Host::Host()
{
	m_OfxHost.host = reinterpret_cast<OfxPropertySetHandle>(this);
	m_OfxHost.fetchSuite = SFN_FetchSuite;

	m_PluginCache.Rescan();
	m_PluginCache.InitializeAllPlugins(m_OfxHost);
}

void ofx::Host::OnNewFrame(obs_source_frame *frame)
{
	using namespace std::chrono;

	// OpenFX expects pixels in RGB(A) format going from bottom left to top right.
	// Currently OBS orders pixels from top left to bottom right and every component
	// is in a separate array.

	auto start = steady_clock::now();

	obs_source_frame *rgb_frame = create_rgba_from_yuv422_frame(frame);
	obs_source_frame_destroy(frame); frame = nullptr;

	auto end = steady_clock::now();

	//ofx_info("Filter exec time: %d µs",
	//	 duration_cast<microseconds>(end - start).count());
	//ofx_info("Timestamp delta: %ld", rgb_frame->timestamp - m_PrevFrameTimestamp);

	m_PrevFrameTimestamp = rgb_frame->timestamp;

	m_FrameQueue.push(rgb_frame);
}

std::optional<obs_source_frame *> ofx::Host::GetProcessedFrame()
{
	if (!m_FrameQueue.empty()) {
		auto frame = m_FrameQueue.front();
		m_FrameQueue.pop();
		return frame;
	}

	return std::nullopt;
}

void ofx::Host::LoadPlugin(const std::string_view identifier)
{
	auto pluginOpt = m_PluginCache.GetPluginByIdentifier(identifier.data());
	if (!pluginOpt.has_value()) {
		ofx_info("Cannot find plugin %s", identifier);
		return;
	}

	if (m_ActivePlugin != nullptr) {
		ofx_info("Unloading %s", m_ActivePlugin->pluginIdentifier);
		FinalizeLoadedPlugin();
	}

	ofx_info("About to load %s", identifier);
	m_ActivePlugin = pluginOpt.value();

	InitializeActivePlugin();
}

const void *ofx::Host::FetchSuite(const char *suiteName, int suiteVersion)
{
	using namespace ofx::suites;

	if (suiteVersion != 1)
		return nullptr;

	if (strcmp(suiteName, kOfxImageEffectSuite) == 0)
		return nullptr;	// TODO: Implement suite.
	if (strcmp(suiteName, kOfxPropertySuite) == 0)
		return nullptr; // TODO: Implement suite.
	if (strcmp(suiteName, kOfxParameterSuite) == 0)
		return ParameterSuiteV1::GetSuiteDesc();
	if (strcmp(suiteName, kOfxMemorySuite) == 0)
		return MemorySuiteV1::GetSuiteDesc();
	if (strcmp(suiteName, kOfxMultiThreadSuite) == 0)
		return MultiThreadSuiteV1::GetSuiteDesc();

	return nullptr;
}

void ofx::Host::InitializeActivePlugin()
{
	assert(m_ActivePlugin != nullptr);
	m_ActivePlugin->setHost(&m_OfxHost);

	auto pluginMain = m_ActivePlugin->mainEntry;
	pluginMain(kOfxActionLoad, &m_ActiveInstance, nullptr, nullptr);
	pluginMain(kOfxActionDescribe, &m_ActiveInstance, nullptr, nullptr);
	pluginMain(kOfxActionCreateInstance, &m_ActiveInstance, nullptr,
		   nullptr);
}

void ofx::Host::FinalizeLoadedPlugin()
{
	assert(m_ActivePlugin != nullptr);

	auto pluginMain = m_ActivePlugin->mainEntry;
	pluginMain(kOfxActionDestroyInstance, &m_ActiveInstance, nullptr,
		   nullptr);
	pluginMain(kOfxActionUnload, &m_ActiveInstance, nullptr, nullptr);

	m_ActivePlugin = nullptr;
}
