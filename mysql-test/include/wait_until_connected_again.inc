#
# Include this script to wait until the connection to the
# server has been restored or timeout occurs
--disable_result_log
--disable_query_log
let $counter= 500;
let $mysql_errno= 9999;
while ($mysql_errno)
{
  # Strangely enough, the server might return "Too many connections"
  # while being shutdown, thus 1040 is an "allowed" error
  # See BUG#36228

  # WSREP: allow error 1205, as SHOW STATUS may fail if
  #        wsrep_sync_wait is enabled
  --error 0,1040,1053,1205,2002,2003,2006,2013
  show status;

  dec $counter;
  if (!$counter)
  {
    --die Server failed to restart
  }
  --sleep 0.1
}
--enable_query_log
--enable_result_log

# WSREP: SHOW STATUS queries are allowed even if wsrep
#        is not ready. Make sure wsrep is ready before
#        returning from this script

--source include/wait_wsrep_ready.inc
