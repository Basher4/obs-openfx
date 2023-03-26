#include <chrono>

#include "../logging.h"
#include "Host.h"

ofx::Host::Host() {}

void ofx::Host::OnNewFrame(obs_source_frame *frame) {
	using namespace std::chrono;

	auto start = steady_clock::now();

	for (int plane = 0; plane < 3; plane++) {
		for (size_t y = 0; y < frame->height; y++) {
			for (size_t x = 0; x < frame->linesize[plane] / 2;
			     x++) {
				size_t idx = y * frame->linesize[plane] + x;
				size_t ridx = (y + 1) * frame->linesize[plane] -
					      x - 1;

				uint8_t tmp = frame->data[plane][idx];
				frame->data[plane][idx] =
					frame->data[plane][ridx];
				frame->data[plane][ridx] = tmp;
			}
		}
	}

	auto end = steady_clock::now();

	ofx_info("Filter exec time: %d µs",
		 duration_cast<microseconds>(end - start).count());
	ofx_info("Timestamp delta: %ld", frame->timestamp - prev_timestamp);

	ofx_info("Color matrix:");
	for (int i = 0; i < 4; i++) {
		ofx_info("%.3f %.3f %.3f %.3f", frame->color_matrix[i * 4 + 0],
			 frame->color_matrix[i * 4 + 1],
			 frame->color_matrix[i * 4 + 2],
			 frame->color_matrix[i * 4 + 3]);
	}

	ofx_info();
	prev_timestamp = frame->timestamp;

	frame_queue.push(frame);
}

std::optional<obs_source_frame *> ofx::Host::GetProcessedFrame()
{
	if (!frame_queue.empty()) {
		auto frame = frame_queue.front();
		frame_queue.pop();
		return frame;
	}

	return std::nullopt;
}
