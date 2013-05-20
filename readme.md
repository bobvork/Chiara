# Pebble Chiara Watchface #

This is my first Pebble watchface. I combined the code of a few examples to get a simple watch that displays the time as 4 digits (24 hours). The result is a sort of military format (2100 for nine PM).

![A Mockup of the Chiara Pebble Watchface](/Chiara.png "Chiara")

Some considerations:

* The time is displayed using images rather than using a font. Pebble seems to have trouble displaying fonts in sizes above ~44.
* This watchface only supports a 24 hour clock because there's no place to display AM / PM
* The date is currently in European format (Day-Month), and is very small. I Like it this way, but I might add more options in the future.





## Installation instructions

To install, look for Chiara on [My Pebble Faces][mpf]. The easiest way is to get the .pbw binary there.

If you want to build this source yourself, you'll need the Pebble SDK (PebbleKit). You'll need an account to get the download

* [Pebble SDK][sdk]
* [Installation instructions][sdkinstructions]


[mpf]: http://www.mypebblefaces.com
[sdk]: https://account.getpebble.com/sdk_releases
[sdkinstructions]: http://developer.getpebble.com/1/01_GetStarted/index




## TODO

There are still some optimisations I would like to do:

* Only update parts of the display that have to be updated. This should decrease system usage (and improve battery life)
* More options for the date (US format, bigger font size)

## Attributions

The font used for the time is Fairview. You can find it at Lost Type Co-op:

[http://www.losttype.com/fairview/?name=fairview][fairview]

[fairview]: http://www.losttype.com/fairview/?name=fairview

## License

Do whatever you want with this. I mostly copied stuff and moved it around anyway.