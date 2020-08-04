# bwapi-tm
A tournament module for BWAPI bots. This is useful for operating competitions that run a large-number of bot-vs-bot games. It can also be used to operate human-vs-bot games.

## State file
The tournament module updates a state file that the calling system can read or monitor to determine the progress of each game, and may help to adjudicate the winner of each game. By default, the tournament module updates the state file every 5 seconds, given the opportunity.

The fields in the state file contain win/loss conditions, timers information, and various other information. The number of fields is predictable, but it depends on how many kinds of per-frame timeout the current player is using. See `TMState::write(std::string filename)` in `TournamentModule.cpp` to see the list of fields. Some fields are described in the `README.md` file in the `Source` folder. Notes:
* How to determine which player is the winner/loser: see the `README.md` in the `Source` folder. The logic is the same for all BWAPI versions.
* The `TM_GAME_FRAME_LIMIT` option can be used to limit the maximum number of game frames per game.
* The `gameElapsedTime` field is the total game wall clock time so far, and by default there is no specific limit for this, although the `TM_GAME_TIME_LIMIT` option can be used to limit it.
* The `selfScore` field is calculated using AIIDE rules, but all the individual broken-down fields such as `selfKillScore` & `selfRazingScore` & `selfBuildingScore` & `selfGatheredMinerals` & `selfGatheredGas` etc are stored in case you want to calculate score differently.
* There are also some timing stats like `totFrameTime`, `avgFrameTime`, `maxFrameTime`, `minFrameTime`. `minFrameTime` is initially `999999`.
* There's a `randomSeed` field. This information might be useful in some cases to more consistently reproduce crashes/timeouts etc later, via the `seed_override` option in `bwapi.ini` when trying to rerun a game to reproduce the problem. It is only available for BWAPI 4.2.0 and above. In earlier versions of BWAPI it is set to `0`.

## Options
You can use environment variables and/or a configuration file to control options, e.g. environment variables such as `TM_LOCAL_SPEED=42` and `TM_DISABLE_USER_INPUT=false`, and/or a configuration file whose path defaults to `./bwapi-data/tm_settings.ini` relative to the Starcraft process' current working directory. The options in this file (if used) have the same names as the corresponding environment variables. To see the default values and documentation for each option, see `tm_settings.ini` under the folder for the source code, and the variants for particular common usages such as bot-vs-bot play, human-vs-bot play, and headless bot-vs-bot play.

## How to use
To use this project, be sure to specify the path of this tournament module DLL such as `tournament = bwapi-data\TournamentModule.dll` or `tournament = bwapi-data\TM\TournamentModule.dll` in `bwapi.ini` for the appropriate player(s), depending on the path that you want BWAPI to load it from. If a BWAPI bot is used to control the local player, the version of BWAPI used to compile this tournament module should correspond to the version of BWAPI used by the local bot, otherwise the TM/bot may not start or may crash etc due to incompatibility. On the machine running the bot, the BWAPI version that was used to compile bwapi.dll, TournamentModule.dll and the bot has to match. It's impossible to know whether this TM would work properly with a custom or incorrect BWAPI version. It might work, it might not, or it might seem to work but get crashes or strange problems sometimes, so it is discouraged.

If used, the value of the per-frame timeouts option (`TM_TIMEOUTS`) may need to be tweaked for some bots, depending on how slowly those bot(s) are on the local system's hardware/OS. Some bots might often time out if run on a system that is slower than they were tested on.

Warning: if you have a process that monitors the state file produced by this project, it's possible that the the state file may be written then rewriten shortly afterwards, so if Starcraft is still running, your system should detect and wait a little while & retry, perhaps a few times, to read the file if the file is deleted or shortened or changes etc underneath it.

## Notes about BWAPI version differences
Where possible, this tournament module is intended to behave the same way regardless of which version of BWAPI is used. At the time of writing (July 2020), the only differences between BWAPI versions that significantly affects this tournament module are that:
* BWAPI didn't time client (proxy) bots properly in versions prior to 4.4.0. This was due to the internal logic of BWAPI (not this tournament module). So, for client bots that use a version of BWAPI below 4.4.0, this tournament module won't detect them timing out (e.g. individual frames taking too many milliseconds). Only client (proxy) bots are adversely affected though - DLL bots are not adversely affected.
* The `randomSeed` field will be set to `0` for BWAPI versions below 4.2.0, because early versions of BWAPI did not provide a way to get the random seed.

## Origin, Licenses and Credits
This project is based on the tournament module from https://github.com/davechurchill/StarcraftAITournamentManager (MIT license), with some features incorporated from https://github.com/basil-ladder/sc-tm (MIT license). See the `LICENSES` file and the `Licenses` folder. I am very grateful to the authors of those projects, of which my project merely enhances.

## Enhancements
For the record, as-at the time of writing (July 2020), the notable differences compared with the tournament module from StarcraftAITournamentManager are as follows. Note: if I make further changes, I do not intend to keep keep this list up to date.

Features:
* Added a `TM_DISABLE_DRAW_GAME_TIMER` option to display an in-game timer. By default the timer is not displayed. To make the TM display the timer, set this option to `false`, and also ensure that the `TM_DRAW_GUI` option is set to `true`.

Enhancements:
* Now supports BWAPI version 3.7.5 (in addition to 4.4.0, 4.2.0, 4.1.2, 3.7.4 that the StarcraftAITournamentManager base already supported).

State file changes:
* Fixed a bug in the setting of the `gameElapsedTime` field - it was almost always `0`. This field is supposed to be set to the number of milliseconds since the game started (wall clock time).
* Added a lot of new fields, e.g. booleans for whether each particular kind of timer threshold has been exceeded, some timer details (total, mean, max, min), various scores info, random seed (for trying to reproduce crashes/timeouts etc later).
* Inserted a version field at the start of the state file (i.e. it's the version of the file format, not the version of the TM), for better future-proofing.
* Inserted a number-of-timeout-fields field into the state file, so that it is easier to parse the file and count the number of fields to calculate whether the file is incomplete.
* Added a `randomSeed` field. This information might be useful in some cases to more consistently reproduce crashes/timeouts etc later, via the `seed_override` option in `bwapi.ini` when trying to rerun a game to reproduce the problem. It is only available for BWAPI 4.2.0 and above. In earlier versions of BWAPI it is set to `0`.

Options:
* Added some new options, and documented all options in the sample settings file (`tm_setting.ini`). All the new option names start with `TM_`, although the old-style option names are still recognized from the settings file. If desired, options in the (optional) settings file can be overridden by environment variables with the same name. Also added an optional `TM_SETTINGS_FILE` environment variable for the location to read a settings file from.

Misc bugfixes:
* Fixed a bug where the magic number 85714 was being used rather than a variable for the game frame limit.

Visual C++ project files:
* Use environment variables such as `BWAPI_420` instead of `BWAPI_DIR` in the Visual C++ project files.
* Minor clean-up of Visual C++ project files, although this was mainly for the Debug configuration which hasn't been used as far as I know.
* Refactor only: Beautified e.g. tabs to spaces and more consistent indenting.

Tidy-up:
* Added guards against some potential null pointer exceptions that wouldn't happen during expected usage (`BWAPI::Broodwar->enemy()`).
* Refactor only: Use `ostringstream` to help avoid delays during updating the state file.
* Refactor only: Decoupled the auto observer more from the tournament AI class, via `AIModule` callbacks.
* Refactor only: More consistent variable naming conventions, e.g. no leading underscore for member variables.