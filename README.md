# bwapi-tm
A tournament module for BWAPI bots. This is useful for operating competitions that run a large-number of bot-vs-bot games. It can also be used to operate human-vs-bot games.

The tournament module updates a state file that the calling system can read or monitor to determine the progress of each game, and may help to adjudicate the winner of each game.

You can use environment variables and/or a configuration file to control options, e.g. environment variables such as `TM_LOCAL_SPEED=42` and `TM_DISABLE_USER_INPUT=false`, and/or a configuration file whose path defaults to `./bwapi-data/tm_settings.ini` relative to the Starcraft process' current working directory. The options in this file (if used) have the same names as the corresponding environment variables.

To use this project, be sure to specify the path of this tournament module DLL such as `tournament = bwapi-data\TournamentModule.dll` or `tournament = bwapi-data\TM\TournamentModule.dll` in `bwapi.ini` for the appropriate player(s), depending on the path that you want BWAPI to load it from.

If used, the value of the per-frame timeouts option (`TM_TIMEOUTS`) may need to be tweaked for some bots, depending on how slowly those bot(s) are on the local system's hardware/OS.

Warning: if you have a process that monitors the state file produced by this project, it's possible that the the state file may be written then rewriten shortly afterwards, so if Starcraft is still running, your system should detect and wait a little while & retry, perhaps a few times, to read the file if the file is deleted or shortened or changes etc underneath it.

The fields in the state file contain win/loss conditions, timers information, and various other information. The number of fields is predictable, but it depends on how many kinds of per-frame timeout the current player is using. See `TMState::write(std::string filename)` in `TournamentModule.cpp` to see the list of fields. Notes:
* How to determine which player is the winner/loser: see the `README.md` in the source code. The logic is the same for all BWAPI versions.
* The `TM_GAME_FRAME_LIMIT` option can be used to limit the maximum number of game frames per game.
* The `gameElapsedTime` field is the total game wall clock time so far, and by default there is no specific limit for this, although the `TM_GAME_TIME_LIMIT` option can be used to limit it.
* The `selfScore` field is calculated using AIIDE rules, but all the individual broken-down fields such as `selfKillScore` & `selfRazingScore` & `selfBuildingScore` & `selfGatheredMinerals` & `selfGatheredGas` etc are stored in case you want to calculate score differently.
* There are also some timing stats like `totFrameTime`, `avgFrameTime`, `maxFrameTime`, `minFrameTime`. `minFrameTime` is initially 999999.
* There's a `randomSeed` field. This information might be useful in some cases to more consistently reproduce crashes/timeouts etc later, via the `seed_override` option in `bwapi.ini` when trying to rerun a game to reproduce the problem. It is only available for BWAPI 4.2.0 and above. In earlier versions of BWAPI it is set to 0.