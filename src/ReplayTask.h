/* -*- Mode: C++; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#ifndef RR_REPLAY_TASK_H_
#define RR_REPLAY_TASK_H_

#include "Task.h"

class TraceFrame;

class ReplayTask : public Task {
public:
  ReplayTask(ReplaySession& session, pid_t _tid, pid_t _rec_tid,
             uint32_t serial, SupportedArch a);

  ReplaySession& session() const;

  /**
   * Call this method when this task has just performed an |execve()|
   * (so we're in the new address space), but before the system call has
   * returned.
   */
  void post_exec(const std::string& replay_exe);

  enum {
    /* The x86 linux 3.5.0-36 kernel packaged with Ubuntu
     * 12.04 has been observed to mutate $esi across
     * syscall entry/exit.  (This has been verified
     * outside of rr as well; not an rr bug.)  It's not
     * clear whether this is a ptrace bug or a kernel bug,
     * but either way it's not supposed to happen.  So we
     * allow validate_args to cover up that bug. */
    IGNORE_ESI = 0x01
  };
  /** Assert that the current register values match the values in the
   *  current trace record.
   */
  void validate_regs(uint32_t flags = 0);
  const TraceFrame& current_trace_frame();
  /** Restore the next chunk of saved data from the trace to this. */
  ssize_t set_data_from_trace();
  /** Restore all remaining chunks of saved data for the current trace frame. */
  void apply_all_data_records_from_trace();
  /**
   * Set the syscall-return-value register of this to what was
   * saved in the current trace frame.
   */
  void set_return_value_from_trace();

private:
  ~ReplayTask() {}
};

#endif /* RR_REPLAY_TASK_H_ */
