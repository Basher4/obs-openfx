#pragma once

#include <chrono>
#include <optional>
#include <queue>

#include <obs.h>

namespace ofx {
class Host {
private:
	uint64_t prev_timestamp;
	std::queue<obs_source_frame *> frame_queue;

public:
	Host();
	~Host() = default;

	void OnNewFrame(obs_source_frame *frame);
	std::optional<obs_source_frame *> GetProcessedFrame();
};
}
