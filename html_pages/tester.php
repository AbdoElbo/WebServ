<?php
// 1. Status code
$status = $_GET['status'] ?? $argv[1] ?? 400;
$status = (int) $status;
http_response_code($status); // 200 OK (this is also the default)

// 2. Headers (must come before any output)
header('Content-Type: text/html; charset=UTF-8');
header('X-Powered-By-Me: my-first-app'); // a custom header, just as an example

// 3. Body (anything you echo or put outside the PHP tags)
?>
<!DOCTYPE html>
<html>
<head>
  <title>Basic Response</title>
</head>
<body>
  <h1>Hello!</h1>
  <p>Server time: <?php echo date('H:i:s'); ?></p>
</body>
</html>