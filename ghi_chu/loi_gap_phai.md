# Nhat ky loi AS2 (dung cho bao cao: Muc 2 race condition da phat hien + Reflection)

| Ngay | Trieu chung | Nguyen nhan | Cach debug | Cach sua |
|---|---|---|---|---|
| 27/10 | Build ARM: `undefined reference to __aeabi_ldivmod` | Chia so 64-bit (s64) tren ARM 32-bit, kernel khong co ham ho tro cua libgcc | Doc thong bao linker | Dung `div_s64()` |
| 28/10 | Jitter SCHED_FIFO rat cao, gan bang SCHED_OTHER | Driver lay moc `last_read_ms = now` moi lan doc nen lech dan, read() bi cho them, deadline cua app bi tre | So sanh jitter truoc/sau (`logs/jitter_before_fix.txt`) | Tien moc theo luoi: `last_read_ms += interval_ms`, chi dat lai = now khi tre qua 2 chu ky |
| 28/10 | Host: `pthread_create` tra EAGAIN | `mlockall(MCL_FUTURE)` + gioi han RLIMIT_MEMLOCK khien khong cap duoc stack thread | Doc ma loi, thu bo mlockall | Chi goi mlockall khi co `-r` |
| 30/10 | Sensor loi (khong co device) nhung app van `exit=0` va chay het thoi gian | Thread sensor return ma khong bao cho main | Chay `-D /khong/ton/tai; echo $?` | Them `error` vao `sensor_args`; dat `a->error = 1; *a->running = 0;` o 6 duong loi |
| 30/10 | Valgrind: `mq_timedsend(msg_ptr) points to uninitialised byte(s)`, 15 errors (`raw/2026-10-30_valgrind_truoc_fix.log`) | `mq_send` gui du 256 byte, `snprintf` chi ghi den `\0`; `alarm_msg` co 4 byte padding sau enum | `--track-origins=yes` chi ra bien stack trong `processor_thread_func` | `memset` struct truoc khi dien du lieu (3 cho) -> 0 errors |
| 30/10 | `fake_sensor.sh` sinh timestamp 19 chu so | `date` ban uutils bo qua `%3N` | So sanh voi `/tmp/sms.log` | `$(( $(date +%s%N) / 1000000 ))` |
