# Seed

Provides simple seeding (Instant Win) functionality to other modules. 
This is handy when building "we're giving away *n* amount of prizes" type sites.

## Getting started:
1. Enable the module.
1. Use **drush seed-generate-random "now" "+1 month" "1 day"** to generate random seeds within a time period (start time, end time, random window)
1. Add "use seeds" permission to user appropriate user role.
1. Use seed_find() to find a seed.
1. Use seed_use() to use (redeem) the seed.
1. Use seed_drop() to drop a seed.

## Advanced:

* Built in test block for easy testing of functionality.
* Built in views integration (mostly useful for administrative interfaces).
* Basic services integration with the seed_service module.
* Supports time limited seeds.
* Supports expirations on found seeds.
* Supports replanting of seeds so they can be found again, or they can be removed from circulation after dropping.
* Supports use of taxonomy to categories seeds.

## Note:

* Currently there is no UI for this module, initial work has started on seed_ui but it's not ready. Drush commands have been set up to generate and clear seeds.
