@ECHO OFF

REM xsubst application.
REM Copyright (c) 2004-2011 by Elijah Zarezky,
REM All rights reserved.

REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM
REM http://www.apache.org/licenses/LICENSE-2.0
REM
REM Unless required by applicable law or agreed to in writing, software
REM distributed under the License is distributed on an "AS IS" BASIS,
REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM See the License for the specific language governing permissions and
REM limitations under the License.

REM irbuild.cmd - NT command script to perform incremental release build

nant.exe -f:xsubst.2008.build @NAntSettings.txt build-all