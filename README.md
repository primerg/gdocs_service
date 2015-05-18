# GdocService #
Convenient wrapper to get files and folders using Google Drive API.
It works with the existing Google API PHP Client. See requirements.

## Requirements ##
* [PHP 5.2.1 or higher](http://www.php.net/)
* [Google API PHP Client](https://github.com/google/google-api-php-client)

## EXAMPLE ##

```php
$client = new Google_Client();
...

$doc = new GdocService($client);

$folders = $doc->getFolders();

foreach ($folders as $folder) {
$files = $doc->getFiles($folder['id']);

foreach ($files as $file) {
  $html = $doc->getFileContent($file);
  echo $html;
}
}
```