<h1>Account Profile</h1>
<form action="profile.php" method="post">
    <fieldset>
        <div class="form-group">
            <input autofocus class="form-control" name="current_pw" placeholder="Current Password" type="password">
        </div>
        <div class="form-group">
            <input class="form-control" name="new_pw" placeholder="New Password" type="password"/>
        </div>
        <div class="form-group">
            <input class="form-control" name="confirm_pw" placeholder="Confirm New Password" type="password"/>
        </div>
        <div class="form-group">
            <button type="submit" class="btn btn-default">Change Password</button>
        </div>
    </fieldset>
</form>
