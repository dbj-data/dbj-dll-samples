@cls
@echo off

pushd .
pushd .\A
call .\build-a.cmd
popd
popd

pushd .
pushd .\dbj-syserrmsg
call .\build-dbj-syserrmsg.cmd
popd
popd

pushd .
pushd .\dbj-shmem
call .\build-dbj-shmem.cmd
popd
popd

pushd .
pushd .\dbj-vector
call .\build-dbj-vector.cmd
popd
popd

pushd .
pushd .\host_console_app
call .\build-console-host.cmd
popd
popd
