Built using Microsoft Visual Studio Express 2013 for Windows Desktop.

On the machine where you are compiling, you must set the BWAPI_412 environment variable to the path of your BWAPI 4.1.2 folder, so that it can find include files and LIB files etc.

A sample settings file is provided in "tm_settings.ini". This file is not used when compiling, and the existence of this file is optional when running the Tournament Module.

These fields in the state file for each player ("gameState.txt" by default) can be used to determine the winner:
* gameOver: 0 or 1. It is 1 iff onEnd() has happened yet.
* isWinnerInOnEnd: 0 or 1. Corresponds to the argument from onEnd(bool isWinner) (if it has happened yet; if it hasn't happened yet it is 0) but I vaguely recall that some old versions of BWAPI provided an incorrect value, and looking at this value alone doesn't take into account whether any timeouts have been exceeded, so I suggest you avoid relying on this value where possible.
* defeated: 0 or 1. It is 1 if all your buildings were destroyed. Note: if you left the game but not all your buildings were destroyed, this will be 0.
* victorious: 0 or 1. It is 1 if all the enemy buildings were destroyed or the enemy left the game.
* exceededTimeoutsLimit: 0 or 1. If it is 1 it means that at some point before onEnd() it would be treated as being disqualified due to too many per-frame timeouts (however, this might have happened after a game frame limit or game wall clock time limit had already been exceeded).
* frameCountTimeoutsExceeded: The frame number of the first occasion that the per-frame timeout limits was exceeded (0 if not used or not exceeded yet).
* exceededElapsedTimeLimit: 0 or 1. Whether the game wall clock time limit was exceeded (0 if this limit is not used or not exceeded yet).
* frameCountElapsedTimeExceeded: The frame number of the first occasion that the game wall clock time limit was exceeded (0 if this limit is not used or not exceeded yet).
* frameCount: Frame number of the latest update to the state file.
* exceededFrameCountLimit: 0 or 1. Whether the game frame limit was exceeded (0 if this limit is not used or not exceeded yet).
* frameCountFrameCountLimitExceeded: The frame number of the first occasion that the game frame limit was exceeded (0 if this limit is not used or not exceeded yet).
* selfScore or the other scores breakdown fields. Note: enemyScore is normally 0 because CompleteMapInformation is not used. To get the enemy's score, you need to look in the other player's state file.

Here is the proposed logic to fairly determine the winner, based on whatever information is available in the state files:

Wait until all the Starcraft processes and other relevant processes have terminated.

Check that the state file exists for each player and contains the correct number of fields.

if (both players have missing/incomplete state file)
{
    The winner can't be determined fairly. There might be a system problem (might not be the bot's fault, e.g. out of memory/disk) or Starcraft or the bot(s) crashed in frame 0, or the bot was killed while it was rewriting the state file. Maybe don't count this game at all? Stop.
}

if (exactly one player has missing/incomplete state file)
{
    There's no record of one of the bots starting, so there might be a system problem (might not be the bot's fault, e.g. out of memory/disk) or Starcraft or the bot crashed in frame 0, or the bot was killed while it was rewriting the state file. Maybe don't count this game at all? Stop.
}

A player is immediately disqualified due to exceeding the timeouts limits if their state file has (exceededTimeoutsLimit == 1 && (exceededFrameCountLimit == 0 || frameCountTimeoutsExceeded <= frameCountFrameCountLimitExceeded) && (exceededElapsedTimeLimit == 0 || frameCountTimeoutsExceeded <= frameCountElapsedTimeExceeded)), and if this happened, they were disqualified on frame number frameCountTimeoutsExceeded and if you display a column for "Timeout?" it should say "Yes" (it may also have crashed later - that will be determined separately). If both players were disqualified, the player that has the lowest frameCountTimeoutsExceeded is the loser and the other is the winner. If both players were disqualified but they have the same frameCountTimeoutsExceeded then break the tie as follows, otherwise stop.

If player(s) have gameOver == 0 it means it/they did not get as far as the onEnd() callback. If both bots have gameOver == 0 it may mean that process(es) were killed for both bots or there was some kind of system problem that affected both bots (e.g. unexpected power outage, system shutdown, out of disk space/memory), or the parent process(es) were terminated (an upstream bug?), or Starcraft crashed for both bots. It is also possible that both bot(s) crashed at about the same time. So, it might be unclear whether it is the fault of the bots. If it happens for a match between two bots that generally rarely or never crash/fail then this is a red flag that it might be a system problem rather than a problem with the bots, so is worth investigating, because it might be unfair to mark it/them as having crashed. If only one bot has gameOver == 0 it is less likely to be a system problem, so you should probably treat that bot as having crashed, e.g. if you have a column for "Crash?" it should say "Yes". In which case, the bot that has the highest frameCount should probably be the winner. If they have the same frameCount then break the tie as follows, otherwise stop.

if (one or both players have (exceededFrameCountLimit == 1 || exceededElapsedTimeLimit == 1))
{
    The winner is decided based on score, and the game should be marked e.g. "Game frame limit exceeded" or "Game time limit exceeded" or both. Stop.
}

if (one player has (defeated == 0 && victorious == 1) and the other player has (victorious == 0))
{
    That player is the winner. Stop.
}

if (one player has (defeated == 1 && victorious == 0) and the other player has (defeated == 0))
{
    That player is the loser. Stop.
}

if (one player has (isWinnerInOnEnd == 1) and the other player has (isWinnerInOnEnd == 0))
{
    That player is the winner. Stop.
}

Otherwise, the winner is decided based on score. If they have the same score, you might be able to break the tie if frameCount is different for each player (which may happen if Starcraft or bots(s) crashed or timed out etc), i.e. the player with the highest frameCount is the winner. There may be some luck involved with how often each player's state file is updated though, because by default it's not updated every frame. If they have the same frameCount then maybe assign the winner randomly.
