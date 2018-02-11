<?php
include_once 'db_credentials.php';
include_once 'utilityFunctions.php';
include_once 'groupFunctions.php';
include_once 'whiteboardFunctions.php';
include_once 'utilityFunctions.php';



function updateFlashCards($connection, $ip, $clients, $groupID, $sock) {


  $connection = connectGroup();
  //SQL Commands
  $flashGroupID = "$groupID" . "FC";

  //RETURN FRONT SIDE AND BACK SIDE
  for($i= 0; $i<2; $i++){
    if($i == 0){
      $side = 'side1';
      $code = 'FCFT';
    }
    else if($i == 1){
      $side = 'side2';
      $code = 'FCBK';
    }

    $return_FlashCards = "SELECT id, $side
                        FROM $flashGroupID
                        WHERE id IS NOT NULL";
    $resultFlashCards = mysqli_query($connection, $return_FlashCards); //runs and stores results of all flash cards in the group currently.
    $num_FlashCards = $resultFlashCards->num_rows;//stores number of flashcards
    echo "We are writing to current user joining group: $ip who's socket should be $sock \n";
    echo "Debugging: What is the number of Flashcards: $num_FlashCards \n";
    for($n_FlashCards = $num_FlashCards; $n_FlashCards > 0; $n_FlashCards = $n_FlashCards - 1){ //For loop that iterates through lists of flashcards to writeback to client.
      $row=mysqli_fetch_array($resultFlashCards); //Fetches first flash card into array
      $newID = $row[0] -1;
      $FlashCards = "$newID $row[1]"; //Stores side1 and side2 into variable
      echo "Debugging: We are writing $FlashCards to ip $ip with socket $sock \n";
      $message = "$code$FlashCards"; //Appends CODE FLSH to username
      $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT); //Pads left of code with length of string so client knows how much to read
      fwrite($sock,"{$messageSize}{$message}"); //Writes back to client.
      echo "Debugging: Client should be receiving: {$messageSize}{$message} \n";
    } //closes query for loop
  } //closes front/back for loop
  disconnect($connection);


}//Close function

function addToCard($groupID, $num, $message, $user, $clientList, $sock, $side) {
  $side = "side" . "$side";
  echo "Side variable is: $side.\n";
  $connection =  connectGroup();
  echo "Connect group worked properly.\n";
  $username = $user->getName();
  echo "user getname returned user: $username\n";
  $ip = $user->getIP();
  echo "user getIP returned ip: $ip\n";
  $num = $num + 1;
  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);

  $username = mysqli_real_escape_string($connection, $username);
  $message = mysqli_real_escape_string($connection, $message);
  $groupID = mysqli_real_escape_string($connection, $groupID);
  $flashGroupID = "$groupID" . "FC";

  $return_ipList = "SELECT ipAddress FROM $groupID WHERE ipAddress IS NOT NULL";
  $resultIP = mysqli_query($connection, $return_ipList); //Returns list of current IP addresses i.e. current user list connected.
//  $num_ip = $resultIP->num_rows; //Stores number of people currently connected for while loop iteration.


  $query = "INSERT INTO $flashGroupID (user, $side) VALUES ('$username', '$message')";

// Check to see if the id for this card exists already
  $check_card = "SELECT * FROM $flashGroupID WHERE (id='$num')";

  if (checkExists($connection, $check_card) > 0){
    echo "Card exists already ";
    $update = "UPDATE $flashGroupID SET user= '$username', $side='$message' WHERE (id='$num')";
    mysqli_query($connection, $update);
    $NewID = "SELECT id FROM $flashGroupID WHERE (user='$username' AND $side='$message')";
    $result = mysqli_query($connection, $NewID);
    $obj = $result->fetch_object();
    $returnID = $obj->id; // returnID == return value of NewID
    $returnID = $returnID -1;
    $clientMessage = "SUCC{$returnID}";
    $messageSize = str_pad((string)strlen($clientMessage), 5, "0", STR_PAD_LEFT); //might need tuning
    fwrite($sock, "{$messageSize}{$clientMessage}"); //writes to the socket
    echo "$clientMessage \n";

  while($rowIP = mysqli_fetch_array($resultIP)){
    $keyIP = $rowIP[0];
    $keySock = $clientList[$keyIP]->getSocket();
    $FlashCards = "$returnID $message";

    if($side == 'side1'){
      $clientMessage = "FCFT$FlashCards";
    }
    else if($side == 'side2'){
      $clientMessage = "FCBK$FlashCards";
    }
    $messageSize = str_pad((string)strlen($clientMessage), 5, "0", STR_PAD_LEFT); //Pads left of code with length of string so client knows how much to read
    fwrite($keySock,"{$messageSize}{$clientMessage}"); //Writes back to client.
    echo "in while rowIP = mysqli fetch array \n $clientMessage \n";



  } //Closes while loop
  }//Closes outer if statement

  else{
    mysqli_query($connection, $query);
    $NewID = "SELECT id FROM $flashGroupID ORDER BY id DESC LIMIT 1";
    $result = mysqli_query($connection, $NewID);
    //echo "newID query has run. result is: $result\n\n";
    $obj = $result->fetch_object();
    $returnID = $obj->id;
    $returnID = $returnID -1;
    //echo "returnID is: $returnID\n\n";
    $clientMessage = "SUCC{$returnID}";
    //echo "clientMessage is: $clientMessage\n\n";
    $messageSize = str_pad((string)strlen($clientMessage), 5, "0", STR_PAD_LEFT); //might need tuning
    fwrite($sock, "{$messageSize}{$clientMessage}"); //writes to the socket
    echo "in else statement \n $clientMessage \n";

while($rowIP = mysqli_fetch_array($resultIP)){
  $keyIP = $rowIP[0];
  $keySock = $clientList[$keyIP]->getSocket();
  $FlashCards = "$returnID $message";

  if($side = 'side1'){
    $clientMessage = "FCFT$FlashCards";
  }
  else if($side = 'side2'){
    $clientMessage = "FCBK$FlashCards";
  }

  $messageSize = str_pad((string)strlen($clientMessage), 5, "0", STR_PAD_LEFT); //Pads left of code with length of string so client knows how much to read
  fwrite($keySock,"{$messageSize}{$clientMessage}"); //Writes back to client.
  echo "$clientMessage \n in else statement while loop \n";

}

  } // end else bracket
  disconnect($connection);
}//close function

?>
