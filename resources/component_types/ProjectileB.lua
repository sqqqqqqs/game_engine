ProjectileB = {
	destroyed = false,

	OnStart = function(self)
		self.transform = self.actor:GetComponent("Transform")

		-- Register this enemy in the global table.
	end,

	OnUpdate = function(self)
		local player = Actor.Find("PlayerA"):GetComponent("Transform")
		if self.transform.x - player.x < 10 and abs(self.transform.y - player.y) < 10 then 
			Actor.Destroy(self.actor)
		end
	end,

	OnLateUpdate = function(self)
		
		if self.transform.x < 0 then
			self.destroyed = true
		end
		
		if self.destroyed then
			Actor.Destroy(self.actor)
		end
	end,

	-- This OnDestroy function is not a lifecycle function, and is not automatically called from the engine.
	-- It is called manually by other lua components.
	-- In homework 8, you'll make this an automatically-called lifecycle function.
}

