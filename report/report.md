# 3/27/2020 Update

For this week we were meant to implement the KV Store in a distributed fashion that way it can run across our network. Our implementation is not fully functional but it should be working soon. We have run into issues for pair programming as now we are in different time zones, so coordinating our work schedules is difficult. We hope to resolve our outstanding issues this weekend to help us prepare for the next week. The biggest issue that we managed to fix this week was our issue with circular dependencies. Previously, we experience problems through serialization where we would reference our serial.h file in other places before it was correctly defined. We were able to remedy these issues, allowing our code to properly compile.

Unresolved issues
- Fix serialization
- Finish implementation of KV Store

Going forwards, we plan on finishing what we did not get to when it comes to Milestone 3. This is mostly based upon fixing our issues with having our different simulated systems communicating with one another and running the KV Store properly on each system.