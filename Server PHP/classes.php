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
  var $currGroups = array();
  var $recGroups = array();
  var $favGroups = array();
  var $numFavorite = 5;
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
  function getCurrGroups() {
    return $this->currGroups;
  }
  function getRecentGroups(){
    return $this->recGroups;
  }
  function getFavoriteGroups() {
    return $this->favGroups;
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
  function setGroup($groupID) {
    array_unshift($this->currGroups, $groupID);
  }
  function setRecGroup($groupID) {
    if (in_array($groupID, $this->recGroups)) {
      $key = array_search($groupID, $this->recGroups);
      if ($key != 0){
        if(count($this->recGroups) == $this->numRecent){
          array_unshift($this->recGroups, $groupID);
          array_pop($this->recGroups);
        }
        else
          array_unshift($this->recGroups, $groupID);
      }
    }//if in array
    else {
      if(count($this->recGroups) == $this->numRecent) {
        array_unshift($this->recGroups, $groupID);
        array_pop($this->recGroups, $groupID);
      }
      else
        array_unshift($this->recGroups, $groupID);
    }
  }
  function setFavGroup($groupID) {
    if (count($this->favGroups) == $this->numFavorite) {
      array_unshift($this->favGroups, $groupID);
      array_pop($this->favGroups);
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
  function removeGroup($groupID) {
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
  var $memberIPs = array();

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
    return $this->memberIPs;
  }
  function getAdmin() {
    return $this->admin;
  }
  //Set Functions
  function setGroupID($groupname) {
      $this->groupID = $groupname;
  }
  function setMember($member) {
    array_push($this->members, $member);
  }
  function setMemberIP($ipaddress) {
    array_push($this->memberIPs, $ipaddress);
  }
  function setAdmin($adminName) {
    $this->admin = $adminName;
  }
  function removeMember($username) {
    $key = array_search($username, $this->memFavorited);
    array_splice($this->members, $key, 1);
    array_splice($this->memberIPs, $key, 1);
  }
}

?>
