# bwapi-tm
A tournament module for BWAPI bots. This is useful for operating competitions that run a large-number of bot-vs-bot games. It can also be used to operate human-vs-bot games.

The tournament module updates a state file that the calling system can read or monitor to determine the progress of each game, and may help to adjudicate the winner of each game.

You can use environment variables and/or a configuration file to control options, e.g. environment variables such as `TM_LOCAL_SPEED=42` and `TM_DISABLE_USER_INPUT=false`, and/or a configuration file whose path defaults to `./bwapi-data/tm_settings.ini` relative to the Starcraft process' current working directory. The options in this file (if used) have the same names as the corresponding environment variables. To see the default values and documentation for each option, see `tm_settings.ini` under the folder for the source code, and the variants for particular common usages such as bot-vs-bot play, human-vs-bot play, and headless bot-vs-bot play.

To use this project, be sure to specify the path of this tournament module DLL such as `tournament = bwapi-data\TournamentModule.dll` or `tournament = bwapi-data\TM\TournamentModule.dll` in `bwapi.ini` for the appropriate player(s), depending on the path that you want BWAPI to load it from. If a BWAPI bot is used to control the local player, the version of BWAPI used to compile this tournament module should correspond to the version of BWAPI used by the local bot.

If used, the value of the per-frame timeouts option (`TM_TIMEOUTS`) may need to be tweaked for some bots, depending on how slowly those bot(s) are on the local system's hardware/OS.

Warning: if you have a process that monitors the state file produced by this project, it's possible that the the state file may be written then rewriten shortly afterwards, so if Starcraft is still running, your system should detect and wait a little while & retry, perhaps a few times, to read the file if the file is deleted or shortened or changes etc underneath it.

The fields in the state file contain win/loss conditions, timers information, and various other information. The number of fields is predictable, but it depends on how many kinds of per-frame timeout the current player is using. See `TMState::write(std::string filename)` in `TournamentModule.cpp` to see the list of fields. Notes:
* How to determine which player is the winner/loser: see the `README.md` in the source code. The logic is the same for all BWAPI versions.
* The `TM_GAME_FRAME_LIMIT` option can be used to limit the maximum number of game frames per game.
* The `gameElapsedTime` field is the total game wall clock time so far, and by default there is no specific limit for this, although the `TM_GAME_TIME_LIMIT` option can be used to limit it.
* The `selfScore` field is calculated using AIIDE rules, but all the individual broken-down fields such as `selfKillScore` & `selfRazingScore` & `selfBuildingScore` & `selfGatheredMinerals` & `selfGatheredGas` etc are stored in case you want to calculate score differently.
* There are also some timing stats like `totFrameTime`, `avgFrameTime`, `maxFrameTime`, `minFrameTime`. `minFrameTime` is initially 999999.
* There's a `randomSeed` field. This information might be useful in some cases to more consistently reproduce crashes/timeouts etc later, via the `seed_override` option in `bwapi.ini` when trying to rerun a game to reproduce the problem. It is only available for BWAPI 4.2.0 and above. In earlier versions of BWAPI it is set to 0.

Where possible, this tournament module is intended to behave the same way regardless of which version of BWAPI is used. At the time of writing (July 2020), the only difference between BWAPI versions that significantly affects this tournament module is that BWAPI didn't time client (proxy) bots properly in versions prior to 4.4.0. This was due to the internal logic of BWAPI (not this tournament module). So, for client bots that use a version of BWAPI below 4.4.0, this tournament module won't detect them timing out (e.g. individual frames taking too many milliseconds). Only client (proxy) bots are adversely affected though - DLL bots are not adversely affected.

This project is based on the tournament module from https://github.com/davechurchill/StarcraftAITournamentManager (MIT license), with some features incorporated from https://github.com/basil-ladder/sc-tm (MIT license).