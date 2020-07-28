Instructions and notes specifically for SCHNAIL:

Install the Licenses folder somewhere (anywhere, e.g. into the "SCHNAIL Client" folder) as part of the SCHNAIL installation process. No need to install the source code. If you ever publish any of the TournamentModule.dll files, you'll also need to publish the Licenses folder alongside the TournamentModule.dll file(s). Again, no need to publish the source code.

You can use environment variables and/or a configuration file to control options.  For SCHNAIL, to help avoid people cheating by editing files or adding environment variables, I suggest not using configuration files - just make SCHNAIL set all the environment variables immediately before starting Starcraft. I.E. equivalent to the values in env_vars.schnail_bot_headless.ini or env_vars.schnail_human.ini (or env_vars.schnail_human_with_game_timer.ini) depending on whether it is a bot or a human. If you want to use configuration files, you should be able to use tm_settings.schnail_bot_headless.ini and tm_settings.schnail_human.ini (or tm_settings.schnail_human_with_game_timer.ini) as-is (put the one you want to use to bwapi-data/TM/tm_settings.ini).

Also, neither the bot nor the human should have speed_override configured in bwapi.ini, otherwise people might be able to change the local speed during intense big battles etc during the game e.g. "/speed 100".

I've assumed you want to use TM_STATE_FILE=bwapi-data\TM\gameState.txt

Ensure that the bwapi-data/TM folder exists for the bot and the human before starting Starcraft, because the Tournament Module doesn't create that folder.

Remember to ensure SCHNAIL sets "tournament = bwapi-data\TM\TournamentModule.dll" in bwapi.ini for both players, assuming that's where it lives.

The per-frame timeouts option (TM_TIMEOUTS) defaults to the same timeouts settings as AIIDE/CoG. SSCAIT allows more time, i.e. ">= 320 frames longer than 85 milliseconds" rather than ">= 320 frames longer than 55 milliseconds". PurpleWave times out a lot using the defaults. BASIL doesn't use per-frame timeouts, which is why PurpleWave works fine on BASIL. If you want to add a little leeway for users that have slow machines or for bots like PurpleWave, you might want to try something like 85 instead of 55. It's a trade-off between fairness for bot authors vs wasting users' time if bots are too slow a lot of the time.

I couldn't prevent users from pausing the game, but by default the Tournament Module will immediately resume the game whenever it is paused (if you prefer, this can be disabled via TM_AUTO_RESUME_GAME).

In the end, I decided not to use append-only mode when writing to the state file. Instead, it will rewrite the file from scratch each update, and it updates it roughly every 5 seconds (the period in milliseconds is TM_STATE_FILE_UPDATE_TIME_PERIOD). Warning: it's possible that the Tournament Module may rewrite the state file then rewrite it again shortly afterwards, so if Starcraft is still running, SCHNAIL should detect and wait a little while & retry, perhaps a few times, to read the file if the file is deleted or shortened or changes etc underneath it.

The fields in the state file contain win/loss conditions, timers information, scores information, and player race(s). The number of fields is predictable, but it depends on how many kinds of per-frame timeout the current player is using. See TMState::write(std::string filename) in TournamentModule.cpp to see the list of fields. Notes:
* How to determine which player is the winner/loser: see the README.md in the source code. The logic is the same for all BWAPI versions.
* I've assumed you will not use a maximum number of game frames limit on SCHNAIL, although the TM_GAME_FRAME_LIMIT option can be used to limit it.
* "gameElapsedTime" is the total game wall clock time so far, and by default there is no specific limit for this, although the TM_GAME_TIME_LIMIT option can be used to limit it.
* "selfScore" is calculated using AIIDE rules, which are different to yours, but all the individual fields you might need like "selfKillScore" & "selfRazingScore" & "selfBuildingScore" & "selfGatheredMinerals" & "selfGatheredGas" etc are stored.
* If "enemyRaceInit" is "Unknown" it means the enemy picked Random race in the game lobby. Similarly for "enemyRaceInit" in the other player's state file. This does mean that a human could find out the enemy race if they picked Random race in the game lobby, by looking in starcraft_bundled_forAI/bwapi-data/TM/gameState.txt though. Not sure if that is a problem. I suppose a feature could be added to the Tournament Module to encrypt fields like this - the data would be uploaded to the SCHNAIL server and only decrypted on the SCHNAIL server (not by the SCHNAIL client itself). If you want to discuss, give me a shout.
* There are also some timing stats like "totFrameTime", "avgFrameTime", "maxFrameTime", "minFrameTime". "minFrameTime" is initially 999999.
* There's a randomSeed field. This information might be useful in some cases to more consistently reproduce crashes/timeouts etc, via the seed_override option in bwapi.ini when trying to rerun a game to reproduce the problem. It is only available for BWAPI 4.2.0 and above. In earlier versions of BWAPI it is set to 0.