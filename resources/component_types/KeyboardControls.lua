KeyboardControls = {
	speed = 5,
	jump_power = 350,
	on_ground = false,

	OnStart = function(self)
		self.rb = self.actor:GetComponent("Rigidbody")
		self.anim1 = self.actor:GetComponentByKey("4")
		self.anim2 = self.actor:GetComponentByKey("5")
		self.spriterenderer = self.actor:GetComponent("SpriteRenderer")
	end,

	OnUpdate = function(self)
		-- Horizontal
		self.anim1.enabled = false
		self.anim2.enabled = false
		local horizontal_input = 0
		if Input.GetKey("right") then
			horizontal_input = self.speed
			self.spriterenderer.sprite = "stayright"
			self.anim1.enabled = true
		end

		if Input.GetKey("left") then
			horizontal_input = -self.speed
			self.spriterenderer.sprite = "stayleft"
			self.anim2.enabled = true
		end

		-- Vertical
		local vertical_input = 0

		-- Check on ground
		self.on_ground = false
		ground_object = Physics.Raycast(self.rb:GetPosition(), Vector2(0, 0.8), 1)
		if Input.GetKeyDown("up") or Input.GetKeyDown("space") then
			self.anim1.enabled = false
			self.anim2.enabled = false
			if ground_object ~= nil then
				vertical_input = -self.jump_power
				self.on_ground = true
			end
		end

		if ground_object ~= nil then
			self.on_ground = true
			if self.spriterenderer.sprite == "jumpright" then
				self.spriterenderer.sprite = "stayright"
			elseif self.spriterenderer.sprite == "jumpleft" then
				self.spriterenderer.sprite = "stayleft"
			end
		end

		if self.on_ground~=true then
			Debug:Log("On Ground!!!")
			if self.spriterenderer.sprite == "stayright" then
				self.spriterenderer.sprite = "jumpright"
			elseif self.spriterenderer.sprite == "stayleft" then
				self.spriterenderer.sprite = "jumpleft"
			end
		end

		self.rb:AddForce(Vector2(horizontal_input, vertical_input))
	end
}

