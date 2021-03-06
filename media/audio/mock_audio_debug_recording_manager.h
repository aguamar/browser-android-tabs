// Copyright (c) 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_MOCK_AUDIO_DEBUG_RECORDING_MANAGER_H_
#define MEDIA_AUDIO_MOCK_AUDIO_DEBUG_RECORDING_MANAGER_H_

#include "base/macros.h"
#include "media/audio/audio_debug_recording_manager.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace media {

class MockAudioDebugRecordingManager : public AudioDebugRecordingManager {
 public:
  explicit MockAudioDebugRecordingManager(
      scoped_refptr<base::SingleThreadTaskRunner> task_runner);

  ~MockAudioDebugRecordingManager() override;

  MOCK_METHOD1(
      EnableDebugRecording,
      void(base::RepeatingCallback<void(const base::FilePath&,
                                        base::OnceCallback<void(base::File)>)>
               create_file_callback));
  MOCK_METHOD0(DisableDebugRecording, void());

 private:
  DISALLOW_COPY_AND_ASSIGN(MockAudioDebugRecordingManager);
};

}  // namespace media.

#endif  // MEDIA_AUDIO_MOCK_AUDIO_DEBUG_RECORDING_MANAGER_H_
