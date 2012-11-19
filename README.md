#Seed

**Seed** is a Drupal module that provides simple seeding (Instant Win) functionality to other modules.

##Getting started:
1. Enable the module.
1. Use **drush seed-generate-random "now" "+1 month" "1 day"** to generate random seeds within a time period (start time, end time, random window)
1. Add "use seeds" permission to user appropriate user role.
1. Use seed_find() to find a seed. Use seed_use($seed->sid) to use (redeem) the seed.

##Advanced use:

TODO