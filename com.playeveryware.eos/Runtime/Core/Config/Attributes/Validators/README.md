# Validator Attributes Usage & Application - HON

This directory contains a variety of attributes that can be applied to classes that derive from `Config`. The attributes are mostly implemented to make effective unit testing of a now-obsolete config class `EOSConfig`, but there are a few additional things to be cognizant of:

The following attributes were implemented to affect the user experience surrounding the entry of configuration values:

- `StringValidationAttribute`
- `RegexValidationAttribute`
- `LengthValidationAttribute`

The attributes above were applied to the `productVersion` field member of the `ProductConfig` class, and enable the user to see immediately when they add an invalid value to the product version field. These attributes and the system can (and likely should) be expanded and be applied to other field members of `ProductConfig`, and the various `PlatformConfig` implementing classes - so as to provide a similar user experience in other fields.