<?php
class GdocService {
  private $client;
  private $service;
  private $files;

  public function __construct($client) {
    $this->client = $client;
    $this->service = new Google_Service_Drive($client);
  }

  /**
   * Retrieve HTML content of the file
   *
   * @param string $file_id
   * @return [string] html content of the file
   */
  public function getFileContent($file) {
    $link = $file['download']['text/html'];
    $request = new Google_Http_Request($link, 'GET', null, null);
    $httpRequest = $this->service->getClient()->getAuth()->authenticatedRequest($request);

    if ($httpRequest->getResponseHttpCode() == 200) {
      return $httpRequest->getResponseBody();
    }

    return '';
  }

  /**
   * Retrieve Google docs files inside a folder. Untested for files outside any specific folder
   *
   * @param string $folder_id
   * @return [array] list of files
   */
  public function getFiles($folder_id) {
    $results = array();
    $pageToken = NULL;

    do {
      try {
        $parameters = array();
        
        if ($pageToken) {
          $parameters['pageToken'] = $pageToken;
        }
        
        $files = $this->service->children->listChildren($folder_id, $parameters);

        foreach ($files as $child) {
          $results[] = $this->getInfo($child->getId());
        }

        $pageToken = $files->getNextPageToken();
      } catch (Exception $e) {
        $pageToken = NULL;
      }
    } while ($pageToken);

    $this->files = $results;
    return $results;
  }

  /**
   * Retrieve Google docs folders
   * @return [array] list of folders
   */
  public function getFolders() {
    $results = array();
    $pageToken = NULL;
    $parameters = array();

    do {
      try {
        $parameters['q'] = "mimeType='application/vnd.google-apps.folder'";
        if ($pageToken) {
          $parameters['pageToken'] = $pageToken;
        }

        $files = $this->service->files->listFiles($parameters);

        foreach ($files as $child) {
          $results[] = $this->getInfo($child->getId());
        }

        $pageToken = $files->getNextPageToken();
      } catch (Exception $e) {
        $pageToken = NULL;
      }
    } while ($pageToken);

    return $results;
  }

  /**
   * Print a file's metadata.
   *
   * @param Google_Service_Drive $service Drive API service instance.
   * @param string $fileId ID of the file to print metadata for.
   */
  public function getInfo($fileID) {
    $file = $this->service->files->get($fileID);

    return array(
      'id'          => $file->getId(),
      'title'       => $file->getTitle(),
      'description' => $file->getDescription(),
      'mime'        => $file->getMimeType(),
      'download'    => $file->getExportLinks()
    );
  }

}