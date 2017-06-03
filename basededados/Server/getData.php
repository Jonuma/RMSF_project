<?php session_start(); ?>
<html>
	<head><title>Getting data from database</title></head>
	<body>
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
			$minutes = array();
			$temperature = array();
			$light = array();
			$moisture = array();
						
			//$result = $connection->prepare("Select temperature from measurements");
			//$temperature = $result->execute();
			
			$sql = "select minutes from measurements";
			$min = $connection->query($sql);
			
			$sql = "select temperature from measurements";
			$temp = $connection->query($sql);
			
			$sql = "select light from measurements";		
			$lightt = $connection->query($sql);
			
			$sql = "select moisture from measurements";		
			$moist = $connection->query($sql);
			
			$i = 0;
			//minutes
			if($min != 0){
				foreach($min as $row ){
					echo("<p>".$row['minutes']."</p>");
					$minutes[$i] = $row['minutes'];
					$i++;
					
				}
			}
			
			$i = 0;
			//temperature
			if($temp != 0){
				foreach($temp as $row ){
					echo("<p>".$row['temperature']."</p>");
					$temperature[$i] = $row['temperature'];
					$i++;
					
				}
			}
			$i = 0;
			//light
			if($lightt != 0){
				foreach($lightt as $row ){
					echo("<p>".$row['light']."</p>");
					$light[$i] = $row['light'];
					$i++;
					
				}
			}
			$i = 0;
			//moisture
			if($moist != 0){
				foreach($moist as $row ){
					echo("<p>".$row['moisture']."</p>");
					$moisture[$i] = $row['moisture'];
					$i++;
					
				}
			}
			
			$response['minutes'] = $minutes;
			$response['temperature'] = $temperature;
			$response['light'] = $light;
			$response['moisture'] = $moisture;
			
		//	echo json_encode($minutes);
		//	echo json_encode($temperature);
		//	echo json_encode($light);
		//	echo json_encode($moisture);
			echo json_encode($response);

			$connection=null;
	
		?>
	</body>
</html>