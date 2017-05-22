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
			
			$email = htmlentities($_GET['email'], ENT_QUOTES);
			$pass = htmlentities($_GET['pass'], ENT_QUOTES);
			
			$result = $connection->prepare("Select email from users where email=:email");
			$result->bindParam(':email', $email);
			$result->execute();
			
			if( ($result->rowcount())!=0 ){
				//$response[0]=1;
				$response["error"] = false;
				$response["message"] = "Logged in successfully!";
			}else{
				$response[0]=0;
				$response["error"] = true;
				$response["message"] = "Failed to login!";
			}
			
			
			
		//	if(($result->rowcount())==0){			
			
		//		$sql = "INSERT INTO users VALUES ('$email', '$pass')";
		//		$result = $connection->exec($sql);
			
				//$result = $connection->prepare("insert into users values(:email, :pass);");
				//$result->bindParam(':email', $user);
				//$result->bindParam(':pass', $pass);
				//$result->execute();
				
		//		$response[0]=1;
				//$response["error"] = false;
				//$response["message"] = "Signed in successfully!";
		//	}else{
		//		$response[0]=0;
				//$response["error"] = true;
				//$response["message"] = "Failed to sign in!";
		//	}
			echo json_encode($response);

			$connection=null;
	
		?>
	</body>
</html>