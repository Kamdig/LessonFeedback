# ThingSpeak Setup - LessonFeedback

This document describes how the ThingSpeak channel is configured for the LessonFeedback (Feedback Station Pico W) project.


## Channel Information

Channel Name: LessonFeedback 

Field 1: LessonID

Field 2: VoteValue

Channel ID: 3235193

Write API Key: Stored securely and is shared internally within the team (=.


## Notes 

This channel is used to receive data from Raspberry Pi Pico W.

LessonID is sent in Field 1 and VoteValue in Field 2.

The Write API Key is required when sending data to the channel via the ThingSpeak HTTP API.


