PlayerA = {
	lives = 3,
	iframe_cooldown = 0,

	OnStart = function(self)
		self.spriterenderer = self.actor:GetComponent("SpriteRenderer")
		self.transform = self.actor:GetComponent("Transform")
	end,

	OnUpdate = function(self) 

		-- projectile spawning
		if Input.GetKeyDown("space") then
			new_projectile = Actor.Instantiate("ProjectileA")
			new_projectile_transform = new_projectile:GetComponent("Transform")
			new_projectile_transform.x = self.transform.x
			new_projectile_transform.y = self.transform.y
		end

		-- iframes
		if self.iframe_cooldown > 0 then
			self.iframe_cooldown = self.iframe_cooldown - 1

			-- Damage flash red
			local sin_value_01 = (math.sin(Application.GetFrame() * 0.4) + 1) * 0.5
			self.spriterenderer.g = 255 * sin_value_01
			self.spriterenderer.b = 255 * sin_value_01
			self.spriterenderer.r = 255

		else
			-- Become normal color again
			self.spriterenderer.g = 255
			self.spriterenderer.b = 255
			self.spriterenderer.r = 255
		end


	end,

	TakeDamage = function(self)
		if self.iframe_cooldown <= 0 then
			if game_over == false then
				Audio.Play(1, "ouch", false)
			end
			self.lives = self.lives - 1
			self.iframe_cooldown = 90
		end

		if self.lives <= 0 then
			Actor.Destroy(self.actor)
		end
	end
}

