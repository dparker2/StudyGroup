<?php
class User {
  function __construct() {
    echo "Constructing user class\n";
  }
  function __destruct() {
    echo "Destructing user class\n";
  }
  var $username;
  var $ip;
  var $socket;
  var $email;
  var $id;
  var $currGroups = array();
  var $recGroups = array();
  var $favGroups = array();
  var $following = array();
  var $followers = array();
  var $numFavorite = 25;
  var $numRecent = 5;

  //Get Functions
  function getName() {
    return $this->username;
  }
  function getIP() {
    return $this->ip;
  }
  function getSocket() {
    return $this->socket;
  }
  function getEmail() {
    return $this->email;
  }
  function getID() {
    return $this->id;
  }
  function getCurrGroups() {
    return $this->currGroups;
  }
  function getRecentGroups(){
    return $this->recGroups;
  }
  function getFavoriteGroups() {
    return $this->favGroups;
  }
  function getFollowing() {
    return $this->following;
  }
  function getFollowers() {
    return $this->followers;
  }

  function setName($name) {
    $this->username = $name;
  }
  function setIP($ipAddress) {
    $this->ip = $ipAddress;
  }
  function setSocket($sock) {
    $this->socket = $sock;
  }
  function setEmail($usrEmail) {
    $this->email = $usrEmail;
  }
  function setNumFavorite($num) {
    $this->numFavorite = $num;
  }
  function setNumRecent($num) {
    $this->numRecent = $num;
  }
  function setID($userID) {
    $this->id = $userID;
  }
  function setGroup($groupID) {
    array_unshift($this->currGroups, $groupID);
  }
  function addFollowing($user, $userID) {
    $this->following[$userID] = $user;
  }
  function addFollowers($user, $userID) {
    $this->followers[$userID] = $user;
  }
  function removeFollowing($userID) {
    unset($this->following[$userID]);
  }
  function removeFollowers($userID) {
    unset($this->followers[$userID]);
  }
  function setRecGroup($groupID) {
    if (in_array($groupID, $this->recGroups)) {
      $key = array_search($groupID, $this->recGroups);
      if ($key != 0){
        $count = count($this->recGroups);
        echo "DEBUG: Trying to add to Recent Group this is the count before add: $count\n";
        if(count($this->recGroups) >= $this->numRecent){
          array_unshift($this->recGroups, $groupID);
          $this->recGroups = array_splice($this->recGroups, 0, $this->numRecent);
        }
        else
          array_unshift($this->recGroups, $groupID);
      }
    }//if in array
    else {
      if(count($this->recGroups) >= $this->numRecent) {
        array_unshift($this->recGroups, $groupID);
        $this->recGroups = array_splice($this->recGroups, 0, $this->numRecent);
      }
      else
        array_unshift($this->recGroups, $groupID);
    }
  }
  function setFavGroup($groupID) {
    if (count($this->favGroups) >= $this->numFavorite) {
      array_unshift($this->favGroups, $groupID);
      $this->favGroups = array_splice($this->favGroups, 0, $this->numFavorite);
    }
    else
      array_unshift($this->favGroups, $groupID);
  }
  function removeFavGroup($groupID) {
    if (in_array($groupID, $this->favGroups)) {
      $key = array_search($groupID, $this->favGroups);
      array_splice($this->favGroups, $key, 1);
    }
  }
  function removeCurrGroup($groupID) {
    if (in_array($groupID, $this->currGroups)) {
      $key = array_search($groupID, $this->currGroups);
      array_splice($this->currGroups, $key, 1);
    }
  }
  function clearGroups() {
    $this->recGroups = array();
    $this->currGroups = array();
    $this->favGroups = array();
  }
}

class Group {
  function _construct() {
    echo "Creating Group Class\n";
  }
  function __destruct() {
    echo "Destructing Group Class\n";
  }

  var $groupID;
  var $numMembers;
  var $admin;
  var $members = array();

  //Get Functions
  function getGroupID() {
    return $this->groupID;
  }
  function getNumMembers() {
    $this->numMembers = count($this->members);
    return $this->numMembers;
  }
  function getMembers() {
    return $this->members;
  }
  function getMemberIP() {
    $ipArray = array();
    foreach($this->members as $user) {
      array_push($ipArray, $user->getIP());
    }
    return $ipArray;
  }
  function getAdmin() {
    return $this->admin;
  }
  //Set Functions
  function setGroupID($groupname) {
      $this->groupID = $groupname;
  }
  function addMember($user) {
    array_push($this->members, $user);
  }
  function setAdmin($adminName) {
    $this->admin = $adminName;
  }
  function removeMember($username) {
    if ($username == NULL) {
      return  "DEBUG: Trying to remove null user from group \n";
    }
    $key = array_search($username, $this->members);
    array_splice($this->members, $key, 1);
  }
}

?>
