<?php session_start(); ?>
<html>
	<head><title>Login from the app</title></head>
	<body>
		<h1>LOGIN</h1>
		<?php
		
			$host = "db.ist.utl.pt";
			$user = "ist178094";
			$password = "kier6808";
			$dsn = "mysql:host=$host;dbname=$user";
		
			try{
				$connection = new PDO($dsn, $user, $password);
			}
			catch(PDOException $exception){
				echo("<p>Error: ");
				echo($exception->getMessage());
				echo("</p>");
				exit();
			}
				
			// array for JSON response
			$response = array();
			
			$usern = htmlentities($_GET['usern'], ENT_QUOTES);
			$pass = htmlentities($_GET['pass'], ENT_QUOTES);
			
			$result = $connection->prepare("Select email from users where email=:usern");
			$result->bindParam(':usern', $usern);
			$result->execute();
			
			if(($result->rowcount())==0){			
			
				$sql = "INSERT INTO users VALUES ('$usern', '$pass')";
				$result = $connection->exec($sql);
			
				//$result = $connection->prepare("Insert into users values(:usern, :pass);");
				//$result->bindParam(':usern', $user);
				//$result->bindParam(':pass', $pass);
				//$result->execute();
				
				$response[0]=1;
				//$response["error"] = false;
				//$response["message"] = "Signed in successfully!";
			}else{
				$response[0]=0;
				//$response["error"] = true;
				//$response["message"] = "Failed to sign in!";
			}
			echo json_encode($response);

			$connection=null;
	
		?>
	</body>
</html>